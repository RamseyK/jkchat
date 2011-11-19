//
//  Client.h
//  chatclient
//
//  Created by Ramsey Kant on 11/4/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef chatclient_Client_h
#define chatclient_Client_h

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <list>
#include <map>

#include "Screen.h"
#include "../../shared/Packet.h"
#include "../../shared/ChatMessage.h"

#define SOCKET int
#define INVALID_SOCKET -1

using namespace std;

class Client {
private:
	Screen *sc;

    SOCKET clientSocket;
    struct addrinfo hints, *res;
	int port;
	bool clientRunning;

	void sendData(Packet *pkt);
    
public:
    Client(Screen *s);
    ~Client();
    
	bool initSocket(int p=27000);
    bool attemptConnect();
    void clientProcess();
	void sendChatMsg(string msg);
    void handleRequest(Packet *pkt);
	void disconnect();

	void setClientRunning(bool c) {
		clientRunning = c;
	}

	bool isClientRunning() {
		return clientRunning;
	}
};

#endif
