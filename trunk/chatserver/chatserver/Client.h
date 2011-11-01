//
//  Client.h
//  chatserver
//
//  Created by Keilan Jackson on 10/31/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef chatserver_Client_h
#define chatserver_Client_h

#include <netinet/in.h>
#include <arpa/inet.h>

#define SOCKET int

class Client {
    
private:
    SOCKET clientSocket;
    sockaddr_in clientAddr;
    
    
public:
    Client(SOCKET, sockaddr_in);
    ~Client();
    
    SOCKET getSocket(){
        return clientSocket;
    }
    
    char *getClientIP(){
        return inet_ntoa(clientAddr.sin_addr);
    }
    
};

#endif
