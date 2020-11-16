#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int main()
{
    // Connecting to SQLite Database
    std::cout << "----------------------------------------\n";
    std::cout << "Connecting to database...\n";

    // Creating socket file descriptor
    std::cout << "Creating a socket...\n";

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error opening socket!\n");
        return -1;
    }

    // Setup the host_addr structure for use in bind call server byte order 
    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    // Convert short integer value for port must be converted into network byte order
    serv_addr.sin_port = htons(54000); 
    // Automatically be filled with current host's IP address
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    /* bind() passes file descriptor, the address structure, and the length of the address structure
    This bind() call will bind  the socket to the current IP address on port */
    if (bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error binding to IP/Port!\n");
        return -2;
    }

    /* This listen() call tells the socket to listen to the incoming connections.
    The listen() function places all incoming connection into a backlog queue until accept() call accepts the connection.
    Here, we set the maximum size for the backlog queue to SOMAXCONN or 4096.*/

    if (listen(sockfd, SOMAXCONN) < 0)
    {
        perror("Error listening!\n");
        return -3;
    }

    // Accepting a connection from a client
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST]; // Client's remote name
    char svc[NI_MAXSERV]; // Service (i.e port) the client is connected on

    std::cout << "Waiting for a connection...\n";

    /* This accept() function will write the connecting client's address info into the the address structure 
    and the size of that structure is clilen. The accept() returns a new socket file descriptor for the accepted connection.
    So, the original socket file descriptor can continue to be used for accepting new connections 
    while the new socket file descriptor is used for communicating with the connected client. */

    int clientSocket = accept(sockfd, (sockaddr *)&client, &clientSize);
    if (clientSocket < 0)
    {
        perror("Error with client connecting!\n");
        return -4;
    }

    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    if (getnameinfo((sockaddr *)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0) == 0)
    {
        std::cout << host << " connected on: " << svc << "!\n"; 
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on port: " << ntohs(client.sin_port) << "!\n";
    }
    
    // Close the original socket
    close(sockfd);

    char buff[4096];
    std::string serv_response;

    // While receiving - Display message, echo message
    while (true)
    {
        // Clear buffer
        memset(buff, 0, 4096);

        // Wait for client to send message
        int bytesReceived = recv(clientSocket, buff, 4096, 0);
        if (bytesReceived < 0)
        {
            perror("Error receiving message!\n");
            break;    
        }

        if (bytesReceived == 0) // This is normal
        {
            std::cout << "Client has disconnected.\n";
            break;
        }

        // Display the message received from the client 
        std::string client_message = std::string(buff, 0, bytesReceived);
        std::cout << "Message received: " << client_message << "\n";

        // TODO: - Open DB, Insert the message, Close the DB
        //       - Fill in all the table columns
        sqlite3 *db;
        char *zErrMsg = 0;
        int rc;

        rc = sqlite3_open("event_log.db", &db);

        if(rc)
        {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            return(0);
        } 
        else 
        {
            std::cout << "Opened database successfully\n";
        }

        /* Create SQL statement */
        std::string sql;

        sql = "INSERT INTO event_log (Message, Timestamp) VALUES ('" + client_message + "', CURRENT_TIMESTAMP);";

        /* Execute SQL statement */
        rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

        if( rc != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);

            // Send the message back to the client
            serv_response = "Event logged successfully";
            send(clientSocket, serv_response.c_str(), serv_response.size()+ 1, 0);
            std::cout << "----------------------------------------\n";
        } else {

            // Send the message back to the client
            serv_response = "Event logged successfully";
            send(clientSocket, serv_response.c_str(), serv_response.size()+ 1, 0);

            std::cout << "Event successfully logged!\n";
            std::cout << "----------------------------------------\n";
        }
        sqlite3_close(db);
    }

    // Close the socket
    close(clientSocket);

    return 0;
}
