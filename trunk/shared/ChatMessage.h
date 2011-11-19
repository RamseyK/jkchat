//
//  ChatMessage.h
//  chatserver
//
//  Created by Keilan Jackson on 11/16/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef chatserver_ChatMessage_h
#define chatserver_ChatMessage_h

#include "Packet.h"

class ChatMessage : public Packet {
    
private:
    string name;
    string message;
    
public:
    ChatMessage();
    virtual ~ChatMessage();
    
    virtual byte *create();
    virtual void parse();
    
    void setName(string name);
    string getName();
    void setMessage(string message);
    string getMessage();
    
    
};

#endif
