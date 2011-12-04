/**
   jkchat, Client
   Screen.cpp
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

#include "Screen.h"

/**
 * Screen Constructor
 * Setup all private instance variables including the line buffer
 */
Screen::Screen() {
	// Initialize ncurses
	c = 0;
	row = 0;
	col = 0;
	line = 2;
	input = "";

    initscr();
    timeout(0); // Non blocking getch
	getmaxyx(stdscr, row, col); // Put the maximum row and col of the screen into row and col

	// Setup the screen buffer map. maps a line number to the string at that line
	scbuf = new map<int,string>();

	// Print the initial prompt
	mvprintw(0, 0, "jkchat> ");
}

/**
 * Screen Destructor
 */
Screen::~Screen() {
	// Cleanup ncurses
	scbuf->clear();
	delete scbuf;
	endwin();
}

/**
 * Process Input
 * Perform a non blocking check for new characters in the user's input stream. Handle "return" events
 * 
 * @return non empty string to send to the sever as a chat message. empty if there is nothing to send
 */
string Screen::processInput() {
	string ret = "";
	usleep(1);

	// Get inputted character
	c = getch();

	// User hit return
	if(c == '\n') {
		// Print the user's input to the screen
		printLine(">" + input);

		// Return the input
		ret = input.c_str();

		// Clear the input variable
		input = "";
	} else if(c > 0) { // Update prompt at top of screen
		// Add the inputted character to the input string
		input += c;

		// Display the new prompt with the latest input string
		mvprintw(0, 0, "jkchat> %s", input.c_str());

		// Refresh the screen
		refresh();
	}
	return ret;
}

/**
 * Print Line onto the Screen
 * Output a new line on the screen. Used by both the client and processInput function
 *
 * @param msg String to print out
 */
void Screen::printLine(string msg) {
	// Up the line count
	line++;

	// Bottom of screen reached, clear the screen and screen buffer
	map<int, string>::const_iterator it;
	if(line == row) {
		for (it = scbuf->begin(); it != scbuf->end(); it++){
			mvprintw((it->first), 0, "                                                        ");
		}
		scbuf->clear();
		line = 2;
	}

	// Put the new line into the screen buffer
	scbuf->insert(pair<int, string>(line, msg));

	// Print the screen buffer
	string pr;
	for (it = scbuf->begin(); it != scbuf->end(); it++){
		pr = it->second;
		mvprintw((it->first), 0, "                                                        ");
		mvprintw((it->first), 0, "%s", pr.c_str());
	}

	// Reset the prompt at the top of the screen
	mvprintw(0, 0, "                                                                    ");
	mvprintw(0, 0, "jkchat> ");

	refresh();
}

