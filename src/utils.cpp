#include "utils.h"
#include "request_router.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>

#define BUFFER_SIZE 4096

void *handleRequest(void *client_socket) {
    int sock = *(int *)client_socket;
    free(client_socket);
    char buffer[BUFFER_SIZE] = {0};
    int bytes_read = read(sock, buffer, BUFFER_SIZE);

    if (bytes_read <= 0) {
        close(sock);
        return NULL;
    }

    char method[16], path[256], protocol[16], body[BUFFER_SIZE] = {0};
    sscanf(buffer, "%s %s %s", method, path, protocol);

    char *body_start = strstr(buffer, "\r\n\r\n");
    if (body_start) {
        strcpy(body, body_start + 4);
    }

    printf("handleRequest: Socket %d - Method %s - Path %s\n", sock, method, path);
    routeRequest(sock, method, path, body);

    close(sock);
    return NULL;
}
