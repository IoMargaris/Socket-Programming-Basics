############################
### My Makefile template ###
### Options: 
### - make
### - make Server
### - make Client
### - make clean
############################

# Compiler settings: 
CC = g++
CXXFLAGS = -std=c++11 -Wall
LIBS = -l sqlite3

# Makefile Settings: 
SERVER_TARGET = Server
CLIENT_TARGET = Client
SRCS = server.cpp client.cpp

.PHONY: all
all: SERVER_TARGET CLIENT_TARGET

SERVER_TARGET: server.cpp
	$(CC) $(CXXFLAGS) -o $(SERVER_TARGET) server.cpp $(LIBS)

CLIENT_TARGET: client.cpp
	$(CC) $(CXXFLAGS) -o $(CLIENT_TARGET) client.cpp

.PHONY: clean
clean:
	rm Server Client
