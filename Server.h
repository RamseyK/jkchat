/**
   jkchat, Server
   Server.h
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

#ifndef chatserver_Server_h
#define chatserver_Server_h

#include <iostream>
#include <stdio.h>

#include <sys/event.h> //kqueue
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <map>

#include "Client.h"

#include "../shared/Packet.h"
#include "../shared/ChatMessage.h"
#include "../shared/LoginPacket.h"
#include "../shared/DCPacket.h"
#include "../shared/UserList.h"

#define SOCKET int
#define INVALID_SOCKET -1
#define QUEUE_SIZE 1024

using namespace std;

class Server {
    
private:
	bool canRun;
    SOCKET listenSocket; // Descriptor for the listening socket
    map<SOCKET, Client*> *clientMap; // Maps a Socket descriptor to a pointer to a Client object (connected clients only)
    struct sockaddr_in serverAddr; // Structure for the server address
	int kqfd; // kqueue descriptor
	struct kevent evlist[QUEUE_SIZE]; // Events that have changed (max QUEUE_SIZE at a time)
    
private:
    bool initSocket(int port = 27000);
    void closeSockets();
    void acceptConnection();
    void disconnectClient(Client *, bool notify = true, bool eraseMap = true);
    void handleClient(Client *);
    void sendData(Client *, Packet *pkt);
    Client *getClient(SOCKET);
    void handleRequest(Client *, Packet *pkt);
    void broadcastData(Packet *pkt);
    void updateUserList();
    
public:
    Server();
    ~Server();
    void runServer();
    void stopServer() {
    	canRun = false;
    }

};

#endif
