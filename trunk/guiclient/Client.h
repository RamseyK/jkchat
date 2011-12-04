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

#include "mainwindow.h"

#include "../shared/Packet.h"
#include "../shared/ChatMessage.h"
#include "../shared/LoginPacket.h"
#include "../shared/DCPacket.h"

#define SOCKET int
#define INVALID_SOCKET -1

using namespace std;

class MainWindow;

class Client {
private:
    MainWindow *mw;

    SOCKET clientSocket;
    struct addrinfo hints, *res;
    string ipstr;
    int port;
    bool clientRunning;

    string clientName;

    bool initSocket();
    void sendData(Packet *pkt);
    
public:
    Client(MainWindow *m);
    ~Client();
    
    bool attemptConnect(string ip, int p = 27000);
    void clientProcess();
    void sendChatMsg(string msg);
    void handleRequest(Packet *pkt);
    void disconnect();

    void setName(string n) {
        clientName = n;
    }

    void setClientRunning(bool c) {
        clientRunning = c;
    }

    bool isClientRunning() {
        return clientRunning;
    }

};

#endif
