//
//  Packet.cpp
//  chatserver
//
//  Created by Keilan Jackson on 11/16/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "Packet.h"


Packet::Packet(unsigned int size) : ByteBuffer(size) {
    opCode = 0;
    
}

Packet::Packet(byte *data, unsigned int size) : ByteBuffer(data, size) {
    opCode = get();
}

Packet::~Packet() {
    
}

byte Packet::getOpcode() {
    return opCode;
}



