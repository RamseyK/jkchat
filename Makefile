# Makefile for jkchat

CC = g++
DEBUG = -g -fpermissive
WARN = -Wall
# Debug Flags
DBGFLAGS = $(DEBUG) $(WARN)
# Production Flags
PRODFLAGS = $(WARN)

CLIENT_H   = chatclient/chatclient/Screen.h chatclient/chatclient/Client.h
CLIENT_SRC = chatclient/chatclient/Screen.cpp chatclient/chatclient/Client.cpp chatclient/chatclient/main.cpp

SERVER_H   = chatserver/chatserver/Client.h chatserver/chatserver/Server.h
SERVER_SRC = chatserver/chatserver/Client.cpp  chatserver/chatserver/Server.cpp chatserver/chatserver/main.cpp

client: $(CLIENT_SRC) $(CLIENT_H)
	$(CC) $(DBGFLAGS) -o bin/$@ $(CLIENT_SRC) -lncurses

server: $(SERVER_SRC) $(SERVER_H)
	$(CC) $(DBGFLAGS) -o bin/$@ $(SERVER_SRC)

clean:
	rm -f *.o bin/*.exe *~ \#*

macClean: clean
	rm -Rf *.dSYM
