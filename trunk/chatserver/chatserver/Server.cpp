//
//  Server.cpp
//  chatserver
//
//  Created by Keilan Jackson on 10/31/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Server.h"

Server::Server(){
    listenSocket = INVALID_SOCKET;
    memset(&serverAddr, 0, sizeof(serverAddr)); //Clear the struct
    
    // Zero the file descriptor sets
    FD_ZERO(&fd_master);
    FD_ZERO(&fd_read);
    
    clientMap = new map<SOCKET, Client*>();
}

Server::~Server(){
    closeSockets();
    delete clientMap;
}

bool Server::initSocket(int port){
    //Creaet a handle for the listening socket, TCP
    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listenSocket == INVALID_SOCKET){
        printf("Could not create socket.\n");
        return false;
    }
 
    //Populate the server address structure
    serverAddr.sin_family = AF_INET; // Family: IP protocol
    serverAddr.sin_port = htons(port); // Set the port (convert from host to netbyte order
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Let the OS select our address
    
    //Bind: assign the address to the socket
    if(bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) != 0){
        printf("Failed to bind to the address\n");
        return false;
    }
    
    //Listen: put the socket in a listening state, ready to accept connections
    // Accept a backlog of the OS Maximum connections in the queue
    if(listen(listenSocket, SOMAXCONN) != 0){
        printf("Failed to put the socket in a listening state\n");
        return false;
    }
    
    //Add the listenSocket to the master fd list
    FD_SET(listenSocket, &fd_master);
    //Set max fd to listenSocket
    fdmax = listenSocket;
    
    return true;
}

void Server::acceptConnection(){
    sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    SOCKET clfd = INVALID_SOCKET;
    
    //Accept pending connection and retrieve the client socket descriptor.  Bail if accept failed.
    clfd = accept(listenSocket, (sockaddr*)&clientAddr, (socklen_t*)&clientAddrLen);
    if (clfd == INVALID_SOCKET) {
        return;
    }
    
    //Creating a new Client object, passing in the Socket handle and client IP address
    Client *cl = new Client(clfd, clientAddr);
    
    //Add to the master FD set
    FD_SET(clfd, &fd_master);
    
    //If the client's handle is greater than the max, set the new max
    if(clfd > fdmax)
        fdmax = clfd;
    
    //Add the client object to the client map
    clientMap->insert(pair<int, Client*>(clfd, cl));
    
    //Print connection message
    printf("%s has connected\n", cl->getClientIP());
    
}

void Server::runServer(){
    //Initializing the socket
    if (!initSocket()) {
        printf("Failed to set up the server\n");
        return;
    }
    
    while(true){
        //Copy the master set into fd_read for processing
        fd_read = fd_master;
        
        //Populate read_fd with client descriptors that are ready to be read
        if(select(fdmax+1, &fd_read, NULL, NULL, NULL) < 0){
            continue;
        }
        
        //Loop through all the descriptors in the read_fd set and check to see if data needs to be processed
        for(int i=0; i <= fdmax; i++){
            // If SOCKET/int i isn't within the set of descriptors to be read, skip it
            if(!FD_ISSET(i, &fd_read)){
                continue;
            }
            
            // A new client is waiting to be accepted on the listenSocket
            if(i == listenSocket){ // A client is waiting to connect
                acceptConnection();
            } else {
                handleClient(getClient(i));
            }
        }
        
    }
    
    closeSockets(); //Closes all connections to the server
    
}

void Server::disconnectClient(Client *cl){
    if (cl == NULL){
        return;
    }
    
    close(cl->getSocket());
    FD_CLR(cl->getSocket(), &fd_master);
    
    delete cl;
    
}

Client *Server::getClient(SOCKET clfd){
    map<int, Client*>::const_iterator it;
    it = clientMap->find(clfd);
    
    // Client wasn't found
    if (it == clientMap->end()){
        return NULL;
    }
    
    // Returns the Client* corresponding to the SOCKET that was found in clientMap
    return it->second;
}

void Server::closeSockets(){
    map<int, Client*>::const_iterator it;
    for (it = clientMap->begin(); it != clientMap->end(); it++){
        Client *cl = it->second;
        disconnectClient(cl);
    }
    
    // Clear the map
    clientMap->clear();
    
    //Close the listening socket
    shutdown(listenSocket, SHUT_RDWR);
    
    //Release the socket to the OS
    close(listenSocket);
    
}

void Server::handleClient(Client *cl){
    if (cl == NULL) {
        return;
    }
    
    size_t dataLen = 1300;
    char *pData = new char(dataLen);
    
    // Receive data on the wire into pData
    /* TODO: Figure out what flags need to be set */
    int flags = 0; 
    ssize_t lenRecv = recv(cl->getSocket(), pData, dataLen, flags);
    
    //Determine state of client socket and act on it
    if(lenRecv == 0) {
        //Client closed the connection
        printf("Client[%s] has disconnected\n", cl->getClientIP());
        disconnectClient(cl);
    } else if(lenRecv < 0) {
        //Some error occured
        disconnectClient(cl);
    } else {
        //Usable data was received, handle it
        printf("Data received: ");
        for (int i=0; i<lenRecv; i++) {
            printf("%c", pData[i]);
        }
        printf("\n");
        handleRequest(cl, pData, lenRecv);
    }
    
    delete [] pData;
}


void Server::sendData(Client *cl, char *pData, size_t dataLen){
    send(cl->getSocket(), pData, dataLen, 0);
}


void Server::handleRequest(Client *cl, char *pData, size_t dataLen){
    map<int, Client*>::const_iterator it;
    for (it = clientMap->begin(); it != clientMap->end(); it++) {
        sendData(it->second, pData, dataLen);
    }
}











