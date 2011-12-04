# Makefile for jkchat

CC = g++
DEBUG = -g -fpermissive
WARN = -Wall
# Debug Flags
DBGFLAGS = $(DEBUG) $(WARN)
# Production Flags
PRODFLAGS = $(WARN)

SHARED_H = shared/ByteBuffer.h shared/Packet.h shared/ChatMessage.h shared/LoginPacket.h shared/DCPacket.h shared/UserList.h
SHARED_SRC = shared/ByteBuffer.cpp shared/Packet.cpp shared/ChatMessage.cpp shared/LoginPacket.cpp shared/DCPacket.cpp shared/UserList.cpp

CLIENT_H   = chatclient/Screen.h chatclient/Client.h
CLIENT_SRC = chatclient/Screen.cpp chatclient/Client.cpp chatclient/main.cpp

SERVER_H   = chatserver/Client.h chatserver/Server.h
SERVER_SRC = chatserver/Client.cpp  chatserver/Server.cpp chatserver/main.cpp

client: $(SHARED_SRC) $(SHARED_H) $(CLIENT_SRC) $(CLIENT_H)
	$(CC) $(DBGFLAGS) -o bin/$@ $(SHARED_SRC) $(CLIENT_SRC) -lncurses

guiclient: guibuild;

server: $(SHARED_SRC) $(SHARED_H) $(SERVER_SRC) $(SERVER_H)
	$(CC) $(DBGFLAGS) -o bin/$@ $(SHARED_SRC) $(SERVER_SRC)

clean:
	rm -f *.o bin/*.exe *~ \#*

macClean: clean
	rm -Rf *.dSYM
