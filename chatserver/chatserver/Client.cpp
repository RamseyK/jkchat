//
//  Client.cpp
//  chatserver
//
//  Created by Keilan Jackson on 10/31/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Client.h"

Client::Client(SOCKET sd, sockaddr_in addr){
    clientSocket = sd;
    clientAddr = addr;
}

Client::~Client(){
    
}
