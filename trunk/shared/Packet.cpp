/**
   jkchat
   Packet.cpp
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

#include "Packet.h"

/**
 * Packet Constructor (size)
 * Accepts an initial size to preallocate in the backing ByteBuffer.
 * This constructor is used when a new packet needs to be created and built
 * 
 * @param size Size to pass to the backing ByteBuffer. Memory of size will be preallocated
 */
Packet::Packet(unsigned int size) : ByteBuffer(size) {
    opCode = 0;
    
}

/**
 * Packet Constructor (data, size)
 * Accepts an initial byte array and size to populate the backing ByteBuffer of the packet. Reads in the opcode from the byte array
 * This constructor is used when a new PROTOCOL packet is recieved over the wire and must be consumed by the Packet class for further manipulation
 *
 * @param data byte Array of packet to consume in the packet structure
 * @param size Size of the byte array
 */
Packet::Packet(byte *data, unsigned int size) : ByteBuffer(data, size) {
    opCode = get();
}

/**
 * Packet Deconstructor
 */
Packet::~Packet() {
    
}

/**
 * Get Opcode
 * Return's the opcode of the packet
 */
byte Packet::getOpcode() {
    return opCode;
}



