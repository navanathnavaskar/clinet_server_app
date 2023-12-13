#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    // Gets listening port from command line
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <server_listening_port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int server_socket, new_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);
    char buffer[MAX_BUFFER_SIZE] = {0};

    // Creating socket for server
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        error("Socket creation failed");
    }

    // Setting up the server address struct
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(atoi(argv[1]));

    // Binding socket to a specific address and port
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        error("Binding failed");
    }

    // Listening for incoming connections
    if (listen(server_socket, 3) == -1) {
        error("Listening failed");
    }

    printf("Server listening on port %d\n", atoi(argv[1]));

    // Accepting incoming connection
    if ((new_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len)) == -1) {
        error("Acceptance failed");
    }

    // Server keep on listening in this loop
    while (1) {
        // Receiving data from the client
        ssize_t recv_result = recv(new_socket, buffer, MAX_BUFFER_SIZE, 0);
        if (recv_result == -1) {
            error("Receive failed");
        } else if (recv_result == 0) {
            // Client closed the connection
            printf("Client closed the connection\n");
            break;
        }

        printf("Received frame: %s\n", buffer);
        // Clear input buffer after use
        memset(buffer, 0, sizeof(buffer));
        // Acknowledging the frame
        if (send(new_socket, "ACK", 3, 0) == -1) {
            error("Send failed");
        }
    }

    close(server_socket);
    return 0;
}

