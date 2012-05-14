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
    createData = NULL;
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
	createData = NULL;
}

/**
 * Packet Deconstructor
 */
Packet::~Packet() {
    if(createData != NULL) {
		delete createData;
		createData = NULL;
	}
}

/**
 * Get Opcode
 * Return's the opcode of the packet
 */
byte Packet::getOpcode() {
    return opCode;
}

/**
 * Check Create
 * Checks if cached data from previous create() calls is available
 *
 * @param force If force is true, createData should be destroyed and the buffer cleared for an upcoming rebuild in create().
 * @return Return true if cached data is available, false if otherwise
 */
bool Packet::checkCreate(bool force) {
	// If force is true, packet is required to flush and rebuilt itself (cannot use cached data)
	if(force) {
		if(createData != NULL) {
			delete createData;
			createData = NULL;
		}
		clear();
	} else { // If cached data from a previous create() call exists
		if(createData != NULL)
			return true;
	}
	return false;
}

/**
 * Save Create
 * Cache's the current contents of the byte buffer in createData for future calls of create()
 */
void Packet::saveCreate() {
    // Create a byte array to return
    createData = new byte[size()];
    // Set read position to beginning of ByteBuffer
    setReadPos(0);
    // Fill the byte array with the usable data in the ByteBuffer (position 0 to size())
    getBytes(createData, size());
}

