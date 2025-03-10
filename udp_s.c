#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void) {
    int socket_desc;
    struct sockaddr_in server_addr, client_addr;
    char server_message[2000], client_message[2000];
    socklen_t client_size;

    // Clean buffers
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    // Create UDP socket
    socket_desc = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_desc < 0) {
        printf("Error: Could not create socket\n");
        return -1;
    }
    printf("Server: Socket created successfully\n");

    // Set port and IP
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Bind socket
    if (bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Error: Couldn't bind to port\n");
        return -1;
    }
    printf("Server: Binding successful, waiting for messages...\n");

    // Communication loop
    while (1) {
        client_size = sizeof(client_addr);

        // Receive message from client
        if (recvfrom(socket_desc, client_message, sizeof(client_message), 0,
                     (struct sockaddr*)&client_addr, &client_size) < 0) {
            printf("Error: Couldn't receive message\n");
            break;
        }
        printf("Client: %s\n", client_message);

        // If client sends "bye", close connection
        if (strncmp(client_message, "bye", 3) == 0) {
            printf("Server: Closing connection...\n");
            break;
        }

        // Get server response
        printf("Server: Enter message: ");
        fgets(server_message, sizeof(server_message), stdin);
        server_message[strcspn(server_message, "\n")] = 0;

        // Send message back to client
        if (sendto(socket_desc, server_message, strlen(server_message), 0,
                   (struct sockaddr*)&client_addr, client_size) < 0) {
            printf("Error: Sending message failed\n");
            break;
        }

        // If server sends "bye", close connection
        if (strncmp(server_message, "bye", 3) == 0) {
            printf("Server: Closing connection...\n");
            break;
        }

        // Clear buffers
        memset(client_message, '\0', sizeof(client_message));
        memset(server_message, '\0', sizeof(server_message));
    }

    // Close socket
    close(socket_desc);

    return 0;
}
