# Makefile for jkchat

CC = g++
DEBUG = -g -fpermissive
WARN = -Wall
# Debug Flags
DBGFLAGS = $(DEBUG) $(WARN)
# Production Flags
PRODFLAGS = $(WARN)

SHARED_H = shared/ByteBuffer.h
SHARED_SRC = shared/ByteBuffer.cpp

CLIENT_H   = chatclient/chatclient/Screen.h chatclient/chatclient/Client.h
CLIENT_SRC = chatclient/chatclient/Screen.cpp chatclient/chatclient/Client.cpp chatclient/chatclient/main.cpp

SERVER_H   = chatserver/chatserver/Client.h chatserver/chatserver/Server.h
SERVER_SRC = chatserver/chatserver/Client.cpp  chatserver/chatserver/Server.cpp chatserver/chatserver/main.cpp

client: $(SHARED_SRC) $(SHARED_H) $(CLIENT_SRC) $(CLIENT_H)
	$(CC) $(DBGFLAGS) -o bin/$@ $(SHARED_SRC) $(CLIENT_SRC) -lncurses

server: $(SHARED_SRC) $(SHARED_H) $(SERVER_SRC) $(SERVER_H)
	$(CC) $(DBGFLAGS) -o bin/$@ $(SHARED_SRC) $(SERVER_SRC)

clean:
	rm -f *.o bin/*.exe *~ \#*

macClean: clean
	rm -Rf *.dSYM
