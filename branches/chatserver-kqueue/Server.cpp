/**
   jkchat, Server
   Server.cpp
   Copyright 2011 Ramsey Kant, Keilan Jackson

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "Server.h"

/**
 * Server Constructor
 * Initialize state and server variables
 */
Server::Server() {
	canRun = false;
    listenSocket = INVALID_SOCKET;
    memset(&serverAddr, 0, sizeof(serverAddr)); // Clear the address struct
    
	// Instance the clientMap, relates Socket Descriptor to pointer to Client object
    clientMap = new map<SOCKET, Client*>();
}

/**
 * Server Destructor
 * Closes all active connections and deletes the clientMap
 */
Server::~Server() {
	if(listenSocket != INVALID_SOCKET)
		closeSockets();
    delete clientMap;
}

/**
 * Init Socket
 * Initialize the Server by requesting a kqueue & socket handle, binding, and going into a listening state
 *
 * @param port Port to listen on
 * @return True if initialization succeeded. False if otherwise
 */
bool Server::initSocket(int port) {	
    // Request a handle for the listening socket, TCP
    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listenSocket == INVALID_SOCKET){
        printf("Could not create socket.\n");
        return false;
    }

	// Set socket as non blocking
	fcntl(listenSocket, F_SETFL, O_NONBLOCK);
 
    // Populate the server address structure
    serverAddr.sin_family = AF_INET; // Family: IP protocol
    serverAddr.sin_port = htons(port); // Set the port (convert from host to netbyte order
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Let the OS select our address
    
    // Bind: assign the address to the socket
    if(bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) != 0){
        printf("Failed to bind to the address\n");
        return false;
    }
    
    // Listen: put the socket in a listening state, ready to accept connections
    // (SOMAXCONN) Accept a backlog of the OS Maximum connections in the queue
    if(listen(listenSocket, SOMAXCONN) != 0){
        printf("Failed to put the socket in a listening state\n");
        return false;
    }

    // Setup kqueue
	kqfd = kqueue();
	if(kqfd == -1) {
		printf("Could not create the kernel event queue\n");
		return false;
	}
	
	// Have kqueue track the listen socket
	struct kevent kev;
	EV_SET(&kev, listenSocket, EVFILT_READ, EV_ADD, 0, 0, NULL); // Fills in the kevent struct
	kevent(kqfd, &kev, 1, NULL, 0, NULL); // Add the watch
    
    canRun = true;

    return true;
}

/**
 * Accept Connection
 * When a new connection is detected in runServer() this function is called. This attempts to accept the pending connection, instance a Client object, and add to the client Map
 */
void Server::acceptConnection() {
	// Setup new client variables
    sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    SOCKET clfd = INVALID_SOCKET;
    
    // Accept pending connection and retrieve the client socket descriptor.  Bail if accept failed.
    clfd = accept(listenSocket, (sockaddr*)&clientAddr, (socklen_t*)&clientAddrLen);
    if (clfd == INVALID_SOCKET)
        return;

	// Set socket as non blocking
	fcntl(clfd, F_SETFL, O_NONBLOCK);
    
    // Creating a new Client object, passing in the Socket handle and client IP address
    Client *cl = new Client(clfd, clientAddr);
    
	// Have kqueue track the new client socket (udata contains a pointer to the Client object)
	struct kevent kev;
	EV_SET(&kev, clfd, EVFILT_READ, EV_ADD, 0, 0, cl); // Fills in the kevent struct
	kevent(kqfd, &kev, 1, NULL, 0, NULL); // Add the watch
    
    // Add the client object to the client map
    clientMap->insert(pair<int, Client*>(clfd, cl));
    
    // Print connection message
    printf("%s has connected\n", cl->getClientIP());
    
}

/**
 * Run Server
 * Main server loop where the socket is initialized and the loop is started, checking for new messages or clients to be read with select()
 * and handling them appropriately
 */
void Server::runServer() {
    //Initializing the socket
    if (!initSocket()) {
        printf("Failed to set up the server\n");
        return;
    }

	printf("jkchat Server has started successfully!\n\n");
	
	int nev = 0; // Number of changed events returned by kevent
	Client *cl = NULL;

    while(canRun) {
		// Get a list of changed socket descriptors (if any) in evlist
        // Timeout is NULL, kevent will wait for a change before returning
		nev = kevent(kqfd, NULL, 0, evlist, QUEUE_SIZE, NULL);
        
        // 0 = timeout, -1 = failure, check errno
        if(nev <= 0)
            continue;
        
        // Loop through only the sockets that have changed in the evlist array
        for(int i = 0; i < nev; i++) {
            if(evlist[i].ident == (unsigned int)listenSocket){ // A client is waiting to connect
                acceptConnection();
            } else {
				cl = (Client*)evlist[i].udata;
				if(evlist[i].flags & EVFILT_READ) {
                	handleClient(cl);
				} else if(evlist[i].flags & EV_EOF) {
					disconnectClient(cl);
				} else {
					// unhandled event
				}
            }
        }
        
    }

    closeSockets(); //Closes all connections to the server
}

/**
 * Disconnect Client
 * Close the client's socket descriptor and release it from the FD map, client map, and memory
 *
 * @param cl Pointer to Client object
 * @param notify If true, sends a user list update to all connected clients
 * @param eraseMap Used to control wheather or not the function should remove the client from the clientMap. If the map is being accessed by the calling function, it may be desireable to not modify the map here
 */
void Server::disconnectClient(Client *cl, bool notify, bool eraseMap) {
    if (cl == NULL)
        return;
    
	// Close the socket descriptor (will also remove from tracked kqueue fd's)
    close(cl->getSocket());
    
	// Remove from the clientMap
    if(eraseMap)
        clientMap->erase(cl->getSocket());
    
	// Free client object from memory
    delete cl;
    
    // Refreshes each client's user list
    if(notify) 
		updateUserList();
}

/**
 * Get Client
 * Lookup client based on the socket descriptor number in the clientMap
 *
 * @param clfd Client Descriptor
 * @return Pointer to Client object if found. NULL otherwise
 */
Client *Server::getClient(SOCKET clfd) {
	// Lookup in map
    map<int, Client*>::const_iterator it;
    it = clientMap->find(clfd);
    
    // Client wasn't found, return NULL
    if (it == clientMap->end())
        return NULL;
    
    // Return a pointer to the Client corresponding to the SOCKET that was found in clientMap
    return it->second;
}

/**
 * Close Sockets
 * Close all sockets found in the client map. Called on server shutdown
 */
void Server::closeSockets() {
	printf("Closing all connections and shutting down the listening socket..\n");

	// Loop through all client's in the map and disconnect them
    map<int, Client*>::const_iterator it;
    for (it = clientMap->begin(); it != clientMap->end(); it++) {
        Client *cl = it->second;
        disconnectClient(cl, false, false); // Don't notify clients of the disconnects
    }
    
    // Clear the map
    clientMap->clear();
    
    // Shutdown the listening socket
    shutdown(listenSocket, SHUT_RDWR);
    
    // Release the listenSocket to the OS
    close(listenSocket);
    listenSocket = INVALID_SOCKET;
}

/**
 * Handle Client
 * Recieve data from a client that has indicated (via select()) that it has data waiting. Pass recv'd data to handleRequest()
 * Also detect any errors in the state of the socket
 *
 * @param cl Pointer to Client that sent the data
 */
void Server::handleClient(Client *cl) {
    if (cl == NULL)
        return;
    
    size_t dataLen = 1300;
    char *pData = new char[dataLen];
    
    // Receive data on the wire into pData
    /* TODO: Figure out what flags need to be set */
    int flags = 0; 
    ssize_t lenRecv = recv(cl->getSocket(), pData, dataLen, flags);
    
    // Determine state of client socket and act on it
    if(lenRecv == 0) {
        // Client closed the connection
        printf("Client[%s] has disconnected\n", cl->getClientIP());
        disconnectClient(cl);
    } else if(lenRecv < 0) {
        // Some error occured
        disconnectClient(cl);
    } else {        
        Packet *pkt = new Packet((byte *)pData, (unsigned int)lenRecv);
        handleRequest(cl, pkt);
        delete pkt;
    }
    
    delete [] pData;
}

/**
 * Handle Request
 * Handle a Packet from a Client recv'd over the wire and act based on protcol specification. Called from handleClient()
 *
 * @param cl Pointer to Client that sent the Packet
 * @param pkt Pointer to Packet containing data recv'd
 */
void Server::handleRequest(Client *cl, Packet *pkt){
    byte opCode = pkt->getOpcode();
    switch (opCode) {
        case OP(LOGIN): {
			// LoginPacket recieved. Parse and check the version number
			LoginPacket *loginPkt = new LoginPacket();
			loginPkt->put(pkt); // Place all the data in the Packet into the newly created chatPkt
			loginPkt->setReadPos(1); // Set the read position for the buffer to be after the opCode byte
			loginPkt->parse();

			// Make sure the protocol version of the client matches the server
			if(loginPkt->getVersion() != PROTOCOL_VERSION) {
				// Create a disconnect packet with the version mismatch as the reason
				DCPacket *dc = new DCPacket();
				dc->setReason("Client and Server version do not match!");
				cout << "[" << cl->getClientIP() << "] has been refused, version mismatch. C: " << loginPkt->getVersion() << ". S: " << PROTOCOL_VERSION << endl;

				// Send the disconnect to the client
				sendData(cl, dc);

				delete dc;
				break;
			}

			// Get the username and set it in the Client object
			cl->setUsername(loginPkt->getUsername());
            
            // Updates the user list with the new client
            updateUserList();
        }
            break;
        case OP(CHAT): {
			// A chat message was recieved. Create an instance of ChatMessage and parse it
            ChatMessage *chatPkt = new ChatMessage();
            chatPkt->put(pkt); // Place all the data in the Packet into the newly created chatPkt
            chatPkt->setReadPos(1); // Set the read position for the buffer to be after the opCode byte
            chatPkt->parse();

			// Display chat message in the console
            cout << chatPkt->getName() << ": " << chatPkt->getMessage() << endl;

			// Broadcast chat message to all connected clients
            broadcastData(chatPkt);

            delete chatPkt;
		}
            break;
        case OP(DC): {
			// Client has signaled a disconnect over the protocol
			DCPacket *dcPkt = new DCPacket();
			dcPkt->put(pkt); // Place all the data in the Packet into the newly created chatPkt
			dcPkt->setReadPos(1); // Set the read position for the buffer to be after the opCode byte
			dcPkt->parse();

			cout << "[" << cl->getClientIP() << "] has disconnected for the following reason: " << dcPkt->getReason() << endl;

			// Perform a hard disconnect now (map removal, close handle)
            disconnectClient(cl);

			delete dcPkt;
		}
            break;
        default:
			cout << "[" << cl->getClientIP() << "] sent an unknown opcode " << opCode << endl;
            break;
    }
}

/**
 * Send Data
 * Send Packet data to a paricular Client
 *
 * @param cl Client to send data to
 * @param pkt Packet containing data to be sent
 */
void Server::sendData(Client *cl, Packet *pkt) {
	// Get the raw byte array by asking the Packet to build itself with create()
    byte *pData = pkt->create();
	if(pData == NULL)
		return;

	size_t totalSent = 0, bytesLeft = pkt->size(), dataLen = pkt->size();
    ssize_t n = 0;

	// Solution to deal with partials sends...loop till totalSent matches dataLen
	while(totalSent < dataLen) {
		n = send(cl->getSocket(), pData+totalSent, bytesLeft, 0);

		// Client closed the connection
		if(n < 0) {
			printf("Client[%s] has disconnected\n", cl->getClientIP());
			disconnectClient(cl);
			break;
		}

		// Adjust byte count after a successful send
		totalSent += n;
		bytesLeft -= n;
	}
}

/**
 * Broadcast Data
 * Sends a Packet to all client's in the clientMap
 *
 * @param pkt Pointer to a Packet to send
 */
void Server::broadcastData(Packet *pkt) {
    map<int, Client*>::const_iterator it;
    for (it = clientMap->begin(); it != clientMap->end(); it++) {
        sendData(it->second, pkt);
    }
}

/**
 * Update User List
 * Creates a UserList packet with the list of connected users and broadcasts it to all connected clients
 */
void Server::updateUserList() {
    UserList *listPkt = new UserList();

	// Loop through the client map and add each client's name to the UserList packet's internal list
    map<int, Client*>::const_iterator it;
    for (it = clientMap->begin(); it != clientMap->end(); it++) {
        listPkt->addUser(it->second->getUsername());
    }
    
	// Send updated UserList to all connected clients
    broadcastData(listPkt);

    delete listPkt;
}
