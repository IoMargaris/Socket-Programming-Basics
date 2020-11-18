import socket

print ("#--------------------#")
print ("Creating socket...")
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_socket.bind(("", 54000))

server_socket.listen(5)

print ("#--------------------#")
print ("Waiting for connection...")
client_socket, addr = server_socket.accept() 
print ("Connection request accepted from:", addr[1])

while True: 
    msg_recv = client_socket.recv(4096)
    print ("#--------------------#")
    print ("Message received: ", msg_recv.decode('utf-8'))

    response = "Thank you for connecting!"
    client_socket.send(response.encode('utf-8')) 

# Close the connection with the client 
client_socket.close()