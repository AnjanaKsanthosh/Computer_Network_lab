#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> // For close()

int main(void) {
    int socket_desc, client_sock, client_size;
    struct sockaddr_in server_addr, client_addr;
    char server_message[2000], client_message[2000];

    // Clean buffers
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc < 0) {
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Server: Socket created successfully\n");

    // Set port and IP
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);  // Port Number
    server_addr.sin_addr.s_addr = inet_addr("127.0.1.1");  // Localhost

    // Bind the socket
    if (bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Error: Couldn't bind to the port\n");
        return -1;
    }
    printf("Server: Binding successful\n");

    // Listen for clients
    if (listen(socket_desc, 1) < 0) {
        printf("Error while listening\n");
        return -1;
    }
    printf("Server: Listening for connections...\n");

    // Accept an incoming connection
    client_size = sizeof(client_addr);
    client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);
    if (client_sock < 0) {
        printf("Error: Can't accept connection\n");
        return -1;
    }
    printf("Server: Client connected (IP: %s, PORT: %i)\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    // Communication loop
    while (1) {
        // Receive message from client
        if (recv(client_sock, client_message, sizeof(client_message), 0) < 0) {
            printf("Error: Couldn't receive message\n");
            break;
        }
        printf("Client: %s\n", client_message);

        // If client sends "bye", close connection
        if (strncmp(client_message, "bye", 3) == 0) {
            printf("Server: Closing connection...\n");
            break;
        }

        // Respond to client
        printf("Server: Enter message: ");
        fgets(server_message, sizeof(server_message), stdin);
        server_message[strcspn(server_message, "\n")] = 0; // Remove newline

        // Send response to client
        if (send(client_sock, server_message, strlen(server_message), 0) < 0) {
            printf("Error: Sending message failed\n");
            break;
        }

        // If server sends "bye", close connection
        if (strncmp(server_message, "bye", 3) == 0) {
            printf("Server: Closing connection...\n");
            break;
        }

        // Clear buffers for the next iteration
        memset(client_message, '\0', sizeof(client_message));
        memset(server_message, '\0', sizeof(server_message));
    }

    // Close the sockets
    close(client_sock);
    close(socket_desc);

    return 0;
}

