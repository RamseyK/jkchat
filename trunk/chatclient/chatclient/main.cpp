/**
   jkchat, Client
   main.cpp
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
#include "Screen.h"
#include "Client.h"

int main (int argc, const char * argv[])
{
	// Create the Screen (ncurses)
	Screen *sc = new Screen();

	// Instantiate a Client object
	Client *cl = new Client(sc);

	// Initialize client socket
	if(!cl->initSocket())
		return -1;

	// Connect to the server
	if(!cl->attemptConnect())
		return -1;

	// Main Program Loop
	string msg;
	cl->setClientRunning(true);

	while(cl->isClientRunning()) {
		// Check to see if theres a new chat message from the user
		msg = sc->processInput();
		if(msg.size() > 0)
			cl->sendChatMsg(msg);

		// Client Main loop
		cl->clientProcess();
	}

	// Disconnect and shutdown the socket
	cl->disconnect();

	delete cl;
	delete sc;

    return 0;
}

