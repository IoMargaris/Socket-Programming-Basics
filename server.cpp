#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

int main()
{
    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "ERROR opening socket!\n";
        return -1;
    }

    // Bind the socket to an ip address and port number
    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(54000); //host to network short conversion
    inet_pton(AF_INET, "0.0.0.0", &serv_addr.sin_addr); // This is our binding information 

    if (bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "ERROR binding to IP/Port!\n";
        return -2;
    }

    // Mark the socket for listening in
    if (listen(sockfd, SOMAXCONN) < 0)
    {
        std::cerr << "ERROR listening!\n";
        return -3;
    }

    // Accept a connection from a client
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST]; // Client's remote name
    char svc[NI_MAXSERV]; // Service (i.e port) the client is connected on

    int clientSocket = accept(sockfd, (sockaddr *)&client, &clientSize);
    if (clientSocket < 0)
    {
        std::cerr << "ERROR with client connecting!\n";
        return -4;
    }

    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    int result = getnameinfo((sockaddr *)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);

    if (result)
    {
        std::cout << host << " connected on: " << svc << "!\n"; 
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on port: " << ntohs(client.sin_port) << "!\n";
    }
    
    // Close the listening socket
    close(sockfd);

    // While reveiving - Display message, echo message
    char buf[4096];
    while (true)
    {
        // Clear buffer
        memset(buf, 0, 4096);

        // Wait for a message
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived < 0)
        {
            std::cerr << "ERROR receiving message!\n";
            break;    
        }

        if (bytesReceived == 0) // This is normal
        {
            std::cout << "Client has disconnected.\n";
            break;
        }

        // Display the message received from the client 
        std::cout << "Client's Message: " << std::string(buf, 0, bytesReceived) << "\n";

        // Send the message back to the client
        send(clientSocket, buf, bytesReceived + 1, 0);
    }

    // Close the socket
    close(clientSocket);

    return 0;
}
