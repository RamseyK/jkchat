//
//  Client.cpp
//  chatclient
//
//  Created by Ramsey Kant on 11/4/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "Client.h"

/**
 * Client Constructor
 * Initializes default values for private members
 */
Client::Client(Screen *s) {
	sc = s;
    clientSocket = INVALID_SOCKET;
    memset(&hints, 0, sizeof(hints));
	port = 27000;
	clientRunning = false;
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
    
	//Determine state of client socket and act on it
	if(lenRecv == 0) {
		// Server closed the connection
		sc->printLine("Socket closed by server, disconnecting\n");
		clientRunning = false;
	} else if(lenRecv == -1) {
		// No data to recv
		return;
	} else {
		//Usable data was received. Make a packet out of the bytes and handle it
        Packet *pkt = new Packet((byte *)pData, (unsigned int)dataLen);
		handleRequest(pkt);
        delete pkt;
	}
    
    delete [] pData;
}

// Takes a packet and handles it based on the opCode
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
        case OP(DC):
            break;
        default:
            break;
    }
}

/**
 * Send Chat Message to the server
 * Take string from the console, wrap it in an appropriate packet, and send it to the server
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
 * @param pData Pointer to a character array of size dataLen to send to the server
 * @param dataLen Size of the character array pData
 */
void Client::sendData(Packet *pkt){
    byte *pData = pkt->create();
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
	freeaddrinfo(res);
	shutdown(clientSocket, SHUT_RDWR);
	close(clientSocket);
    clientSocket = INVALID_SOCKET;

	sc->printLine("Client has disconnected from the server.\n");
}

