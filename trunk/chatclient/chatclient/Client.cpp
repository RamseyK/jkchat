/**
   jkchat, Client
   Client.cpp
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

#include "Client.h"

/**
 * Client Constructor
 * Initializes default values for private members
 *
 * @param s Instance of the Screen class. Client will use this to communicate with the interface
 */
Client::Client(Screen *s) {
	// Set private Screen reference variable (sc) to the s
	sc = s;

    clientSocket = INVALID_SOCKET;
	port = 27000;
	clientRunning = false;

	// Zero out the address hints structure
    memset(&hints, 0, sizeof(hints));
}

/**
 * Client Destructor
 */
Client::~Client() {
	if(clientSocket != INVALID_SOCKET)
		disconnect();
}

/**
 * InitSocket
 * Initialize the socket and put it in a state thats ready for connect()
 *
 * @param port Server port to connect to (default is 27000)
 * @return True if initialization succeeded, false if otherwise
 */
bool Client::initSocket(int p) {
    // Setup the address structure
	port = p;
	char portstr[8];
	sprintf(portstr, "%i", port);
    hints.ai_family = AF_UNSPEC; // Will be determined by getaddrinfo (IPv4/v6)
	hints.ai_socktype = SOCK_STREAM;
	getaddrinfo("127.0.0.1", portstr, &hints, &res);

    // Get a handle for clientSocket
    clientSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(clientSocket == INVALID_SOCKET) {
        sc->printLine("Could not create socket handle");
        return false;
    }

	// At this point, initilization succeeded so return true
	return true;
}

/**
 * Connect
 * Attempt to connect to the target host. Do NOT call if initSocket() failed
 *
 * @return True if connect succeeded, false if otherwise
 */
bool Client::attemptConnect() {
	// Attempt to connect to the server
	stringstream ss;
	ss << "Attempting to connect to 127.0.0.1:" << port << "...";
	sc->printLine(ss.str());
    int ret = 0;
	ret = connect(clientSocket, res->ai_addr, res->ai_addrlen);
	if(ret < 0) {
		sc->printLine("Connect failed!");
		close(clientSocket);
		return false;
	}

	sc->printLine("Connection was successful!");

	// Set as non blocking
	fcntl(clientSocket, F_SETFL, O_NONBLOCK);

	// Send LoginPacket to server
	LoginPacket* loginPkt = new LoginPacket();
	loginPkt->setUsername("");
	sendData(loginPkt);
	delete loginPkt;

	// Otherwise, connect was successful!
	return true;
}

/**
 * Client Process
 * Runs the main checks for new packets and handles any new user input messages in the queue
 *
 */
void Client::clientProcess() {
	size_t dataLen = 1300;
	char *pData = new char[dataLen];
    
	// Receive data on the wire into pData
	/* TODO: Figure out what flags need to be set */
	int flags = 0; 
	ssize_t lenRecv = recv(clientSocket, pData, dataLen, flags);
    
	// Act on return of recv. 0 = disconnect, -1 = no data, else the size to recv
	if(lenRecv == 0) {
		// Server closed the connection
		sc->printLine("Socket closed by server, disconnecting\n");
		clientRunning = false;
	} else if(lenRecv == -1) {
		// No data to recv. Empty case to trap
	} else {
		// Usable data was received. Create a new instance of a Packet, pass it the data from the wire
        Packet *pkt = new Packet((byte *)pData, (unsigned int)dataLen);

		// Pass the new Packet to the Handler
		handleRequest(pkt);
        delete pkt;
	}
    
    delete [] pData;
}

/**
 * Handle Request
 * Accepts an incomming packet from the server, parses it, and performs the appropriate action based on the opcode. Called from clientProcess()
 *
 * @param pkt Pointer to the packet recieved on the wire
 */
void Client::handleRequest(Packet *pkt) {
    byte opCode = pkt->getOpcode();
    switch (opCode) {
        case OP(CHAT): {
            // If the message received is a chat message, just print the name and contents
            ChatMessage *chatPkt = new ChatMessage();
            chatPkt->put(pkt);
            chatPkt->setReadPos(1); // Set the read position for the buffer to be after the opCode byte
            chatPkt->parse();
            sc->printLine(chatPkt->getName() + ": " + chatPkt->getMessage());
            delete chatPkt;
			}
            break;
        case OP(DC): {
			// Server has signaled a disconnect over the protocol
			DCPacket *dcPkt = new DCPacket();
			dcPkt->put(pkt); // Place all the data in the Packet into the newly created chatPkt
			dcPkt->setReadPos(1); // Set the read position for the buffer to be after the opCode byte
			dcPkt->parse();
			
			// Display reason on screen to user
			sc->printLine("Disonnected by Server: " + dcPkt->getReason());

			clientRunning = false;
			delete dcPkt;
			}
            break;
        default:
            break;
    }
}

/**
 * Send Chat Message to the server
 * Take string, wrap it in a ChatMessage and send it to the server
 *
 * @param msg String represenation of the message
 */
void Client::sendChatMsg(string msg) {
	ChatMessage *chatMsg = new ChatMessage();
    chatMsg->setMessage(msg);
	sendData(chatMsg);
	delete chatMsg;
}

/**
 * Send Data
 * Complete a send over the wire to a server.
 *
 * @param pkt Pointer to an instance of a Packet to send over the wire
 */
void Client::sendData(Packet *pkt){
	// Get the raw byte array by asking the Packet to build itself with create()
    byte *pData = pkt->create();
	if(pData == NULL)
		return;

	size_t totalSent = 0, bytesLeft = pkt->size(), dataLen = pkt->size();
    ssize_t n = 0;

	// Solution to deal with partials sends...loop till totalSent matches dataLen
	while(totalSent < dataLen) {
		n = send(clientSocket, pData+totalSent, bytesLeft, 0);

		// Server closed the connection
		if(n < 0) {
			sc->printLine("Error in sending data, socket closed, disconnecting\n");
			clientRunning = false;
			break;
		}

		// Adjust byte count after a successful send
		totalSent += n;
		bytesLeft -= n;
	}
}


/**
 * Disconnect
 * Shutdown and close the socket handle, clean up any other resources in use
 */
void Client::disconnect() {
	// Free the address structure
	freeaddrinfo(res);

	// Shutdown and close the socket, then set in an invalid state
	shutdown(clientSocket, SHUT_RDWR);
	close(clientSocket);
    clientSocket = INVALID_SOCKET;

	sc->printLine("Client has disconnected from the server.\n");
}

