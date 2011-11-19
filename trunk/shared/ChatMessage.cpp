//
//  ChatMessage.cpp
//  chatserver
//
//  Created by Keilan Jackson on 11/16/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "ChatMessage.h"

ChatMessage::ChatMessage() : Packet() {
    opCode = OP(CHAT);
    name = "";
    message = "";
}

ChatMessage::~ChatMessage() {
    
}

//Structure of chat packet:   |opCode|size of name|name|size of message|message|

byte *ChatMessage::create() {
    //Fill ByteBuffer with data in the expected ChatMessage structure
    put(opCode);
    putInt((int)name.size()+1);
    putBytes((byte *)name.c_str(), (int)name.size()+1);
    putInt((int)message.size()+1);
    putBytes((byte *)message.c_str(), (int)message.size()+1);
    
    //Create a byte array to return
    byte *ret = new byte[size()];
    //Set read position to beginning of ByteBuffer
    setReadPos(0);
    //Fill the byte array with the usable data in the ByteBuffer
    getBytes(ret, size());
    return ret;
    
}

void ChatMessage::parse() {
    //Get the size of the username, read it in
    int nameSize = getInt();
    char *n = new char[nameSize];
    getBytes((byte *)n, nameSize);

    
    // Get the size of the message, read it in
    int msgSize = getInt();
    char *msg = new char [msgSize];
    getBytes((byte *)msg, msgSize);

    
    // Set the private variables
    this->name.assign(n);
    this->message.assign(msg);
    
    // Delete the char *s from the heap
    delete [] n;
    delete [] msg;
    
}


string ChatMessage::getName() {
    return name;
}

string ChatMessage::getMessage() {
    return message;
}

void ChatMessage::setName(string name) {
    this->name = name;
}

void ChatMessage::setMessage(string message) {
    this->message = message;
}
