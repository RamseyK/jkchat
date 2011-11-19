//
//  Packet.h
//  chatserver
//
//  Created by Keilan Jackson on 11/16/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef chatserver_Packet_h
#define chatserver_Packet_h

#include "ByteBuffer.h"

enum OP {
    LOGIN   = 0x01,
    CHAT    = 0x02,
    LIST    = 0x03,
    DC      = 0x04
};

class Packet : public ByteBuffer {
    
protected:
    byte opCode;

public:
    Packet(unsigned int size = 4096);
    Packet(byte *data, unsigned int size);
    virtual ~Packet();
    byte getOpcode();
    
    virtual byte *create(){return 0;};
    virtual void parse(){};
    
};



#endif
