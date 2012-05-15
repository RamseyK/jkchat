/**
   jkchat, Server
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

#ifndef chatserver_Client_h
#define chatserver_Client_h

#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

#define SOCKET int

class Client {
    
private:
    SOCKET clientSocket; // Socket Descriptor for the client
    sockaddr_in clientAddr; // Address structure of the client's socket
    std::string username; // Username of the client
    
public:
    Client(SOCKET, sockaddr_in);
    ~Client();
    
    SOCKET getSocket(){
        return clientSocket;
    }
    
    char *getClientIP(){
        return inet_ntoa(clientAddr.sin_addr);
    }
    
	void setUsername(std::string u) {
		username = u;
	}

	std::string getUsername() {
		return username;
	}
};

#endif
