#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <server_port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int client_socket;
    struct sockaddr_in server_address;
    char buffer[MAX_BUFFER_SIZE] = {0};

    // Creating socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        error("Socket creation failed");
    }

    // Setting up the server address struct
    server_address.sin_family = AF_INET;
    // GET port from command line, which is second argument
    server_address.sin_port = htons(atoi(argv[2]));

    // Get IP address from command line, which is first argument
    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, argv[1], &server_address.sin_addr) <= 0) {
        error("Invalid address/ Address not supported");
    }

    // Connecting to the server
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        error("Connection failed");
    }

    // With this loop, client can send messages to server
    // When user enters exit, this loop will break.
    // Client can send message only when remote server
    // acks the message
    while (1) {
        // Get message entered from stdin into buffer
        printf("Enter frame to send (type 'exit' to quit): ");
        fgets(buffer, MAX_BUFFER_SIZE, stdin);

        // Send message to server
        ssize_t send_result = send(client_socket, buffer, strlen(buffer), 0);
        if (send_result == -1) {
            error("Send failed");
        }

        // Check if the user wants to exit
        if (strcmp(buffer, "exit\n") == 0) {
            printf("Exit application...\n");
            break;
        }

        // Clear buffer before receiving Ack from server
        memset(buffer, 0, sizeof(buffer));

        // Receive acknowledgment from the server
        ssize_t recv_result = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);
        if (recv_result == -1) {
            error("Receive failed");
        } else if (recv_result == 0) {
            // Server closed the connection
            printf("Server closed the connection\n");
            break;
        }

        printf("Acknowledgment received from server: %s\n", buffer);
    }

    close(client_socket);
    return 0;
}

