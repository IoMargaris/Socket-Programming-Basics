import socket

print ("#--------------------#")
print ("Creating socket...")
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# 174.138.14.251
print ("#--------------------#")
print ("Connecting to server...")
client.connect(("", 54000))

while True:
    user_input = input("Enter your message: ")
    client.send(user_input.encode('utf-8'))
    server_resp = client.recv(4096)
    print ("Server response: ", server_resp.decode('utf-8'))
    print ("#--------------------#")

client.close()

