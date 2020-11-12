all: Server Client
.PHONY: all

Server: server.cpp
	g++ -o Server server.cpp

Client: client.cpp
	g++ -o Client client.cpp

clean:
	rm Server Client

