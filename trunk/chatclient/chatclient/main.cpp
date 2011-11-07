//
//  main.cpp
//  chatclient
//
//  Created by Keilan Jackson on 10/31/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

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

