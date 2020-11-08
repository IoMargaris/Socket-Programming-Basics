# Client/Server
A simple Client/Server application written in C++ using sockets to send and receive back a message.
<br>

## Steps:
### Compile the server.cpp file
```cpp
g++ server.cpp -o Server
```

### Compile the client.cpp file
```cpp
g++ client.cpp -o Client
```

### Run the Server
```cpp
./Server
```
![Running the Server](./images/running_server.png)

### Run the Client 
```cpp
./Client
```
![Running the Client](./images/running_client.png)

### Observe the server side
![Running the Client, message from server](./images/running_client2.png)


### Type in the message you wish the Client to send. 

*The Server will display the received message and then respond back to the Client with the same message:*

![Sending message, Client Side](./images/sending_message.png)
![Sending message, Server side](./images/sending_message2.png)
![Server Response](./images/server_response.png)