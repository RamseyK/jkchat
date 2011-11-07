//
//  Server.h
//  chatserver
//
//  Created by Keilan Jackson on 10/31/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef chatserver_Server_h
#define chatserver_Server_h

#include <iostream>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <list>
#include <map>

#include "Client.h"

#define SOCKET int
#define INVALID_SOCKET -1

using namespace std;

class Server {
    
private:
    SOCKET listenSocket; // Descriptor for the listening socket
    map<SOCKET, Client*> *clientMap; // Maps a Socket descriptor to a pointer to a Client object (connected clients only)
    struct sockaddr_in serverAddr; // Structure for the server address
    fd_set fd_master; // Master file descriptor set (listening socket + client sockets)
    fd_set fd_read; // FD set of sockets being read/operated on
    int fdmax; // Max FD number (max sockets handle)
    
    
    
private:
    bool initSocket(int port = 27000);
    void closeSockets();
    void acceptConnection();
    void disconnectClient(Client *);
    void handleClient(Client *);
    void sendData(Client *, char *, size_t);
    Client *getClient(SOCKET);
    void handleRequest(Client *, char *, size_t);
    
public:
    Server();
    ~Server();
    void runServer();
    

    
};

#endif
