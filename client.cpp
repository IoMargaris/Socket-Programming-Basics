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
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        return -1;
    }

    // Create a hint structure for the server we are connecting with
    int port = 54000;
    std::string ipAdress = "127.0.0.1";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAdress.c_str(), &hint.sin_addr);

    // Connect to the server on the socket
    int connectRes = connect(sock, (sockaddr *)&hint, sizeof(hint));
    if (connectRes < 0)
    {
        return -1;
    }

    // While loop
    char buf[4096];
    std::string userInput;

    do {
        // Enter lines of text
        std::cout << "> ";
        getline(std::cin, userInput);

        // Send to server
        int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0); // size+1 as we have a trailing zero character at the end of the string
        if (sendResult < 0)
        {
            std::cout << "Cannot send message to server!";
            continue;
        }

        // Wait for response
        memset(buf, 0, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);
        
        // Display response
        std::cout << "SERVER: " << std::string(buf, bytesReceived) << "\r\n";

    } while (true);

    // Close the socket
    close(sock);

    //std::cout << "Hello, World!I will become the Client.\n";
    return 0;
}
