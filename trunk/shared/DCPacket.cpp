/**
   jkchat
   DCPacket.cpp
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

#include "DCPacket.h"

/**
 * DCPacket Constructor
 */
DCPacket::DCPacket() : Packet() {
	reason = "";
}

/**
 * DCPacket Destructor
 */
DCPacket::~DCPacket() {
}

/**
 * DC Create
 * Populate the backing ByteBuffer with the expected DCPacket structure as defined in the protocol
 *
 * Struct:
 * INT: Size of Reason
 * A_STR: Reason for disonnect
 *
 * @return Byte array of the same size as the ByteBuffer (pkt->size()) of the built packet
 */
byte *DCPacket::create() {
	// Build the packet
	put(OP(DC));
	putInt((int)reason.size()+1);
	putBytes((byte *)reason.c_str(), (int)reason.size()+1);

    // Create a byte array to return
    byte *ret = new byte[size()];
    // Set read position to beginning of ByteBuffer
    setReadPos(0);
    // Fill the byte array with the usable data in the ByteBuffer (position 0 to size())
    getBytes(ret, size());

	// Return the created byte array
    return ret;
}

/**
 * DC Parse
 * Parses the DCPacket data structure (after the opcode)
 */
void DCPacket::parse() {
	// Get the size of the reason
	int rSize = getInt();

	// Read the username of size rSize
	char *reas = new char[rSize];
	getBytes((byte*)reas, rSize);
	reason = reas;

	delete [] reas;
}

