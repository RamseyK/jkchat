# Makefile for jkchat

CC = g++

# Debug Flags
DBGFLAGS = -g -O0 -fpermissive -Wall
# Production Flags
PRODFLAGS = -03 -fpermissive -Wall

SHARED_H = shared/ByteBuffer.h shared/Packet.h shared/ChatMessage.h shared/LoginPacket.h shared/DCPacket.h shared/UserList.h
SHARED_SRC = shared/ByteBuffer.cpp shared/Packet.cpp shared/ChatMessage.cpp shared/LoginPacket.cpp shared/DCPacket.cpp shared/UserList.cpp

SERVER_H   = chatserver-epoll/Client.h chatserver-epoll/Server.h
SERVER_SRC = chatserver-epoll/Client.cpp  chatserver-epoll/Server.cpp chatserver-epoll/main.cpp

server: $(SHARED_SRC) $(SHARED_H) $(SERVER_SRC) $(SERVER_H)
	$(CC) $(DBGFLAGS) -o bin/$@ $(SHARED_SRC) $(SERVER_SRC)

clean:
	rm -f *.o bin/* *~ \#*

