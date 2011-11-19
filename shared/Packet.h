/**
   jkchat
   Packet.h
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

#ifndef jkchat_Packet_h
#define jkchat_Packet_h

#include "ByteBuffer.h"

// Protocol Version: Used to ensure that client and server are compiled against the same protocol.
// This should change whenever a compatibility-breaking addition to the protocol is made
#define PROTOCOL_VERSION 1

// Opcodes for use in the jkchat Protocol
// C->S = Client to Server. S->C Server to Client. O = Omnidirectional
enum OP {
    LOGIN   = 0x01, // C->S
    CHAT    = 0x02, // O
    LIST    = 0x03, // S->C
    DC      = 0x04 // O
};

class Packet : public ByteBuffer {
    
protected:
    byte opCode; // Protocol Opcode of the Packet

public:
    Packet(unsigned int size = 4096);
    Packet(byte *data, unsigned int size);
    virtual ~Packet();
    byte getOpcode();
    
    virtual byte *create(){return NULL;};
    virtual void parse(){};
    
};


#endif
