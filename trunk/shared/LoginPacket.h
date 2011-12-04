/**
   jkchat
   LoginPacket.h
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

#ifndef jkchat_LoginPacket_h
#define jkchat_LoginPacket_h

#include "Packet.h"

class LoginPacket : public Packet {
private:
	int version; // Version number parsed
	string username; // Username

public:
	LoginPacket();
	virtual ~LoginPacket();

	virtual byte* create(bool force = false);
	virtual void parse();

	int getVersion() {
		return version;
	}

	string getUsername() {
		return username;
	}

	void setUsername(string u) {
		username = u;
	}

};

#endif