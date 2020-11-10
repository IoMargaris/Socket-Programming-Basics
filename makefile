all: Server Client

Server: server.cpp
	g++ -o Server server.cpp

Client: client.cpp
	g++ -o Client client.cpp

clean:
	rm *.o *.out Server Client

