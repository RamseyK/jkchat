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
byte* LoginPacket::create(bool force) {
	// Check for cached create data
	if(checkCreate(force))
		return createData;

	// Build the packet
	put(OP(LOGIN));
	putInt(PROTOCOL_VERSION); // The protocol version the program was compiled with
	putInt((int)username.size()+1);
	putBytes((byte *)username.c_str(), (int)username.size()+1);

	// Save created data
	saveCreate();

	// Return the created byte array
    return createData;
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

