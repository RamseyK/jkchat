/**
   jkchat, Client
   Screen.h
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

#ifndef chatclient_Screen_h
#define chatclient_Screen_h

#include <iostream>
#include <list>
#include <map>
//#include <ncurses/curses.h> // Cygwin
#include <ncurses.h> // OSX

using namespace std;

class Screen {
private:

	char c; // Character input from user
	int row, col, line; // Screen attributes
	string input; // Full line input from user (aggregation of character input's, c)
	map<int,string> *scbuf; // Screen Buffer map. Relates line number to string of the line

public:
	Screen();
	~Screen();

	string processInput();
	void printLine(string msg);
};

#endif
