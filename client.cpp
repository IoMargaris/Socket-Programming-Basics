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
        return -1;
    }

    // Create a structure for the server we are connecting with (also called a Hint)
    int port = 54000;
    std::string ipAdress = "127.0.0.1";

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, ipAdress.c_str(), &serv_addr.sin_addr);

    // Connect to the server on the socket
    int connectionResult = connect(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr));
    if (connectionResult < 0)
    {
        return -1;
    }

    // While loop
    char buf[4096];
    std::string userInput;

    do {
        // Prompt user to enter lines of text
        std::cout << "> ";
        getline(std::cin, userInput);

        // Send the user input to the server
        int sendResult = send(sockfd, userInput.c_str(), userInput.size() + 1, 0); // size+1 as we have a trailing zero character at the end of the string
        if (sendResult < 0)
        {
            std::cout << "Could not send message to server!";
            continue;
        }

        // Wait for response
        memset(buf, 0, 4096);
        int bytesReceived = recv(sockfd, buf, 4096, 0);
        if (bytesReceived < 0)
        {
            std::cout << "ERROR getting reposonse from Server.";
        }
        else
        {
            // Display response
            std::cout << "SERVER Response: " << std::string(buf, bytesReceived) << "\r\n";
        }
        
    } while (true);

    // Close the socket
    close(sockfd);
    
    return 0;
}
