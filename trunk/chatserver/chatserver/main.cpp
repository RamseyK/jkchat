//
//  main.cpp
//  chatserver
//
//  Created by Ramsey Kant on 10/31/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Server.h"

int main (int argc, const char * argv[])
{

    Server *svr = new Server();
    svr->runServer();
    delete svr;
    return 0;
    
}

