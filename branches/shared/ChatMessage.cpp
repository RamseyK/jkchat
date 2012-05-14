/**
   jkchat
   ChatMessage.cpp
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

#include "ChatMessage.h"

ChatMessage::ChatMessage() : Packet() {
    opCode = OP(CHAT);
    name = "";
    message = "";
}

ChatMessage::~ChatMessage() {
    
}

/**
 * Chat Message Create
 * Populate the backing ByteBuffer with the expected ChatMessage structure as defined in the protocol
 *
 * Struct:
 * INT: Size of the Username
 * A_STR: Username string
 * INT: Size of the chat message
 * A_STR: Message string
 *
 * @return Byte array of the same size as the ByteBuffer (pkt->size()) of the built packet
 */
byte *ChatMessage::create(bool force) {
	// Check for cached create data
	if(checkCreate(force))
		return createData;

    // Build the packet
	put(OP(CHAT));
    putInt((int)name.size()+1);
    putBytes((byte *)name.c_str(), (int)name.size()+1);
    putInt((int)message.size()+1);
    putBytes((byte *)message.c_str(), (int)message.size()+1);

	// Save created data
	saveCreate();

	// Return the created byte array
    return createData;
    
}

/**
 * Chat Message Parse
 * Parses the ChatMessage data structure (after the opcode)
 */
void ChatMessage::parse() {
    //Get the size of the username
    int nameSize = getInt();
	// Read in the username string of size, nameSize
    char *n = new char[nameSize];
    getBytes((byte *)n, nameSize);

    // Get the size of the message
    int msgSize = getInt();
	// Read in the message of size, msgSize
    char *msg = new char [msgSize];
    getBytes((byte *)msg, msgSize);

    // Set the private variables
    this->name.assign(n);
    this->message.assign(msg);
    
    // Delete the char *s from the heap
    delete [] n;
    delete [] msg;
    
}

