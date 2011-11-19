/**
   jkchat
   ChatMessage.h
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

#ifndef jkchat_ChatMessage_h
#define jkchat_ChatMessage_h

#include "Packet.h"

class ChatMessage : public Packet {
    
private:
    string name; // Name of user sending the chat message
    string message; // Chat Message String
    
public:
    ChatMessage();
    virtual ~ChatMessage();
    
    virtual byte *create();
    virtual void parse();

	// Accessors and Mutators:
    
    void setName(string n) {
		name = n;
	}

    string getName() {
		return name;
	}

    void setMessage(string m) {
		message = m;
	}

    string getMessage() {
		return message;
	}
    
};

#endif
