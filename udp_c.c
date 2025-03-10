#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void) {
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[2000], client_message[2000];
    socklen_t server_size;

    // Clean buffers
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    // Create UDP socket
    socket_desc = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_desc < 0) {
        printf("Error: Could not create socket\n");
        return -1;
    }
    printf("Client: Socket created successfully\n");

    // Set server details
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Communication loop
    while (1) {
        // Get user input
        printf("Client: Enter message: ");
        fgets(client_message, sizeof(client_message), stdin);
        client_message[strcspn(client_message, "\n")] = 0;

        // Send message to server
        server_size = sizeof(server_addr);
        if (sendto(socket_desc, client_message, strlen(client_message), 0,
                   (struct sockaddr*)&server_addr, server_size) < 0) {
            printf("Error: Sending message failed\n");
            return -1;
        }

        // If client sends "bye", close connection
        if (strncmp(client_message, "bye", 3) == 0) {
            break;
        }

        // Receive response from server
        if (recvfrom(socket_desc, server_message, sizeof(server_message), 0,
                     (struct sockaddr*)&server_addr, &server_size) < 0) {
            printf("Error: Receiving message failed\n");
            return -1;
        }
        printf("Server: %s\n", server_message);

        // If server sends "bye", close connection
        if (strncmp(server_message, "bye", 3) == 0) {
            printf("Client: Server closed the connection.\n");
            break;
        }

        // Clear buffers
        memset(server_message, '\0', sizeof(server_message));
        memset(client_message, '\0', sizeof(client_message));
    }

    // Close socket
    close(socket_desc);

    return 0;
}
