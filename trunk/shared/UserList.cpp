/**
   jkchat
   UserList.cpp
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

#include "UserList.h"

/**
 * User List Constructor
 */
UserList::UserList() : Packet() {
}

/**
 * User List Destructor
 */
UserList::~UserList() {
}

/**
 * User List Create
 * Populate the backing ByteBuffer with the expected UserList structure as defined in the protocol
 *
 * Struct:
 * INT: Number of Users
 * {
 *	INT: Size of Username
 *	A_STR: Username
 * }
 *
 * @return Byte array of the same size as the ByteBuffer (pkt->size()) of the built packet
 */
byte* UserList::create(bool force) {
	// Check for cached create data
	if(checkCreate(force))
		return createData;

	// Build the packet
	put(OP(LIST));

	// Number of users
	int ls = users.size();
	putInt(ls);

	// Loop through the list and put each user in the packet:
	string username;
	for(int i = 0; i < ls; i++) {
		username = users[i];
		putInt((int)username.size()+1);
		putBytes((byte *)username.c_str(), (int)username.size()+1);
	}

	// Save created data
	saveCreate();

	// Return the created byte array
    return createData;
}

/**
 * UserList Parse
 * Parses the UserList data structure (after the opcode)
 */
void UserList::parse() {
	// Clear out the list
	users.clear();

	// Get the number of users in the list
	int ls = getInt();

	// Read the each username and insert it into the list
	int nameSize = 0;
	for(int i = 0; i < ls; i++) {
		nameSize = getInt();
		char *name = new char[nameSize];
		getBytes((byte*)name, nameSize);
		string username = name;
		users.push_back(username);
		delete [] name;
	}
}

void UserList::addUser(string u) {
	users.push_back(u);
}

int UserList::getListSize() {
	return users.size();
}

string UserList::getUser(int index) {
	return users[index];
}
