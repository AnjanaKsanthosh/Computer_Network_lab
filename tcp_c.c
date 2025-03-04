#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> // For close()

int main(void) {
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[2000], client_message[2000];

    // Clean buffers
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc < 0) {
        printf("Error: Unable to create socket\n");
        return -1;
    }
    printf("Client: Socket created successfully\n");

    // Set port and IP (same as server)
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);  // Port Number
    server_addr.sin_addr.s_addr = inet_addr("127.0.1.1");  // Localhost

    // Connect to server
    if (connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Error: Unable to connect to server\n");
        return -1;
    }
    printf("Client: Connected to server\n");

    // Communication loop
    while (1) {
        // Client sends message first
        printf("Client: Enter message: ");
        fgets(client_message, sizeof(client_message), stdin);
        client_message[strcspn(client_message, "\n")] = 0; // Remove newline

        // Send message to server
        if (send(socket_desc, client_message, strlen(client_message), 0) < 0) {
            printf("Error: Sending message failed\n");
            return -1;
        }

        // If user types 'bye', close the connection
        if (strncmp(client_message, "bye", 3) == 0) {
            break;
        }

        // Receive server response
        if (recv(socket_desc, server_message, sizeof(server_message), 0) < 0) {
            printf("Error: Receiving message failed\n");
            return -1;
        }
        printf("Server: %s\n", server_message);

        // If server sends "bye", close connection
        if (strncmp(server_message, "bye", 3) == 0) {
            printf("Client: Server closed the connection.\n");
            break;
        }

        // Clear buffers for next iteration
        memset(server_message, '\0', sizeof(server_message));
        memset(client_message, '\0', sizeof(client_message));
    }

    // Close the socket
    close(socket_desc);

    return 0;
}

