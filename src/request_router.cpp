#include "request_router.h"
#include "http_handler.h"
#include <string.h>
#include <stdio.h>

void routeRequest(int sock, const char *method, const char *path, const char *body) {
    printf("routeRequest: Method %s - Path %s - Socket %d\n", method, path, sock);
    if (strcmp(method, "GET") == 0) {
        handleRead(sock, path);
    } else if (strcmp(method, "POST") == 0) {
        handlePost(sock, path, body);
    } else if (strcmp(method, "PUT") == 0) {
        handlePut(sock, path, body);
    } else if (strcmp(method, "DELETE") == 0) {
        handleDelete(sock, path);
    } else {
        sendResponse(sock, "405 Method Not Allowed", "text/plain", "405 Method Not Allowed");
    }
}
