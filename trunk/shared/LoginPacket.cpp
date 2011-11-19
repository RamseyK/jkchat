/**
   jkchat
   LoginPacket.cpp
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

#include "LoginPacket.h"

/**
 * Login Packet Constructor
 */
LoginPacket::LoginPacket() : Packet() {
	version = 0;
	username = "";
}

/**
 * Login Packet Destructor
 */
LoginPacket::~LoginPacket() {
}

/**
 * Login Packet Create
 * Populate the backing ByteBuffer with the expected LoginPacket structure as defined in the protocol
 *
 * Struct:
 * INT: Protocol Version
 * INT: Size of Username
 * A_STR: Username
 *
 * @return Byte array of the same size as the ByteBuffer (pkt->size()) of the built packet
 */
byte* LoginPacket::create() {
	// Build the packet
	put(OP(LOGIN));
	putInt(PROTOCOL_VERSION); // The protocol version the program was compiled with
	putInt((int)username.size()+1);
	putBytes((byte *)username.c_str(), (int)username.size()+1);

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
 * Login Packet Parse
 * Parses the LoginPacket data structure (after the opcode)
 */
void LoginPacket::parse() {
	// Get the version number
	version = getInt();

	// Get the size of the username
	int nameSize = getInt();

	// Read the username of size nameSize
	char *name = new char[nameSize];
	getBytes((byte*)name, nameSize);
	username = name;

	delete [] name;
}

