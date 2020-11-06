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
        std::cerr << "Error opening socket!\n";
        return -1;
    }

    // Bind the socket to an IP/Port
    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(54000); //host to network short conversion
    inet_pton(AF_INET, "0.0.0.0", &serv_addr.sin_addr); // This is our binding information 

    if (bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "Error binding to IP/Port!\n";
        return -2;
    }

    // Mark the socket for listening in
    if (listen(sockfd, SOMAXCONN) < 0)
    {
        std::cerr << "ERROR listening!\n";
        return -3;
    }

    // Accept a call
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    int clientSocket = accept(sockfd, (sockaddr *)&client, &clientSize);
    if (clientSocket < 0)
    {
        std::cerr << "ERROR wirh client connecting!\n";
        return -4;
    }

    // Close the listening socket
    close(sockfd);

    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    int result = getnameinfo((sockaddr *)&client,
                            sizeof(client),
                            host,
                            NI_MAXHOST,
                            svc,
                            NI_MAXSERV,
                            0);

    if (result)
    {
        std::cout << host << " connected on: " << svc << "!\n"; 
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on: " << ntohs(client.sin_port) << "!\n";
    }
    
    // While reveiving - Display message, echo message
    char buf[4096];
    while (true)
    {
        // Clear buffer
        memset(buf, 0, 4096); // fill with zeros

        // Wait for a message
        int byteRecv = recv(clientSocket, buf, 4096, 0);
        if (byteRecv < 0)
        {
            std::cerr << "There was a connection issue...\n";
            break;    
        }

        if (byteRecv == 0) // This is normal
        {
            std::cout << "The client disconnected.\n";
            break;
        }

        // Display the message from client 
        std::cout << "Message Received: " << std::string(buf, 0, byteRecv);

        // Resend the message
        send(clientSocket, buf, byteRecv + 1, 0);

    }

    // Close socket
    close(clientSocket);

    //std::cout << "Hello, World! I will become the Server.\n";
    return 0;
}
