# Client/Server
A simple C++ implementation of a Client and Server using sockets. The Client can send a message to the Server and the Server logs the message to a SQLite database and then sends a confirmation response back to the Client.
<br>

### Compile the server.cpp & client.cpp files
```cpp
g++ server.cpp -o Server

g++ client.cpp -o Client
```

### Run the Server first:
```cpp
./Server
```

### Then, run the Client and type your message 
```cpp
./Client
```
