/**
   jkchat, Server
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

#include <iostream>
#include "Client.h"

/**
 * Client Constructor (descriptor, address struct)
 * Constructor that accepts the socket descriptor and the socket's address structure
 *
 * @param sd Socket Descriptor
 * @param addr Address structure for the client's socket
 */
Client::Client(SOCKET sd, sockaddr_in addr){
    clientSocket = sd;
    clientAddr = addr;
	username = "";
}

/**
 * Client Destructor
 */
Client::~Client() {
    
}
