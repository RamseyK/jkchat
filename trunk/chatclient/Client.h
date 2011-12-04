/**
   jkchat, Client
   Client.h
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

#include "../shared/Packet.h"
#include "../shared/ChatMessage.h"
#include "../shared/LoginPacket.h"
#include "../shared/DCPacket.h"

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
