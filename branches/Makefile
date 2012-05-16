# Makefile for jkchat

CC = g++

# Debug Flags
DBGFLAGS = -g -O0 -fpermissive -Wall
# Production Flags
PRODFLAGS = -03 -fpermissive -Wall

SHARED_H = shared/ByteBuffer.h shared/Packet.h shared/ChatMessage.h shared/LoginPacket.h shared/DCPacket.h shared/UserList.h
SHARED_SRC = shared/ByteBuffer.cpp shared/Packet.cpp shared/ChatMessage.cpp shared/LoginPacket.cpp shared/DCPacket.cpp shared/UserList.cpp

SERVER_KQ_H   = chatserver-kqueue/Client.h chatserver-kqueue/Server.h
SERVER_KQ_SRC = chatserver-kqueue/Client.cpp  chatserver-kqueue/Server.cpp chatserver-kqueue/main.cpp

server-kq: $(SHARED_SRC) $(SHARED_H) $(SERVER_KQ_SRC) $(SERVER_KQ_H)
	$(CC) $(DBGFLAGS) -o bin/$@ $(SHARED_SRC) $(SERVER_KQ_SRC)

clean:
	rm -rf *.o bin/* *~ \#*

