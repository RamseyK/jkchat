#ifndef chatclient_Screen_h
#define chatclient_Screen_h

#include <iostream>
#include <list>
#include <map>
#include <ncurses/curses.h>

using namespace std;

class Screen {
private:

	char c;
	int row, col, line;
	string input;
	map<int,string> *scbuf;

public:
	Screen();
	~Screen();

	string processInput();
	void printLine(string msg);
};

#endif
