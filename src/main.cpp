#include "utils.h"
#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BACKLOG 10

int main() {
    int server_fd, new_socket;
    int opt = 1;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create the socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attach the socket to port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Configure the server address
    address.sin_family = AF_INET;            // Use IPv4 protocol
    address.sin_addr.s_addr = INADDR_ANY;    // Accept connections from any network interface
    address.sin_port = htons(PORT);          // Convert port to network format

    // Bind the socket to the network address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Accept a new connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            continue;
        }

        // Create a new thread for each incoming connection
        int *client_socket = (int *)malloc(sizeof(int));
        *client_socket = new_socket;
        pthread_t thread;
        pthread_create(&thread, NULL, handleRequest, client_socket);

        // Detach the thread so it cleans up automatically after execution
        pthread_detach(thread);
    }

    close(server_fd);   // Close the server socket
    return 0;
}
