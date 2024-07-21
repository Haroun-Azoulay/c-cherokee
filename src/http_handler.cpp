#include "http_handler.h"
#include <arpa/inet.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_SIZE 4096
#define ROOT_DIR "www"
#define ROOT_DIR_ACCESS "www/api"

void sendResponse(int sock, const char *status, const char *content_type, const char *body) {
    char response[BUFFER_SIZE];
    int content_length = body ? strlen(body) : 0;

    snprintf(response, sizeof(response),
             "HTTP/1.1 %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %d\r\n"
             "Connection: close\r\n"
             "\r\n"
             "%s",
             status, content_type, content_length, body ? body : "");

    send(sock, response, strlen(response), 0);
}

const char *getContentType(const char *path) {
    const char *ext = strrchr(path, '.');
    if (!ext) return "application/octet-stream";

    if (strcmp(ext, ".html") == 0) return "text/html";
    if (strcmp(ext, ".txt") == 0) return "text/plain";
    if (strcmp(ext, ".json") == 0) return "application/json";
    if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) return "image/jpeg";
    if (strcmp(ext, ".png") == 0) return "image/png";

    return "application/octet-stream";
}

void serveFile(int sock, const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        sendResponse(sock, "404 Not Found", "text/plain", "404 Not Found");
        return;
    }

    struct stat st;
    stat(path, &st);
    int content_length = st.st_size;

    char header[BUFFER_SIZE];
    snprintf(header, sizeof(header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %d\r\n"
             "Connection: close\r\n"
             "\r\n",
             getContentType(path), content_length);

    send(sock, header, strlen(header), 0);

    char buffer[BUFFER_SIZE];
    size_t n;
    while ((n = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(sock, buffer, n, 0);
    }

    fclose(file);
}

void listDirectory(int sock, const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        sendResponse(sock, "404 Not Found", "text/plain", "404 Not Found. You don't have folder www.");
        return;
    }

    struct dirent *entry;
    char body[BUFFER_SIZE] = "<html><body><ul>";

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char fullpath[BUFFER_SIZE];
            snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);
            struct stat st;
            stat(fullpath, &st);

            if (S_ISDIR(st.st_mode)) {
                strcat(body, "<li><b>[DIR]</b> ");
            } else {
                strcat(body, "<li>");
            }
            strcat(body, entry->d_name);
            strcat(body, "</li>");
        }
    }
    strcat(body, "</ul></body></html>");
    closedir(dir);

    sendResponse(sock, "200 OK", "text/html", body);
}

void handleRead(int sock, const char *path) {
    printf("handleRead: Socket %d - Reading path %s\n", sock, path);

    char fullpath[BUFFER_SIZE];
    snprintf(fullpath, sizeof(fullpath), "%s%s", ROOT_DIR_ACCESS, path);

    struct stat st;
    if (stat(fullpath, &st) < 0) {
        sendResponse(sock, "404 Not Found", "text/plain", "404 Not Found");
    } else {
        if (S_ISDIR(st.st_mode)) {
            listDirectory(sock, fullpath);
        } else {
            serveFile(sock, fullpath);
        }
    }
}

void handlePost(int sock, const char *path, const char *body) {
    printf("handlePost: Socket %d - Creating file %s\n", sock, path);

    char fullpath[BUFFER_SIZE];
    snprintf(fullpath, sizeof(fullpath), "%s%s", ROOT_DIR_ACCESS, path);

    FILE *file = fopen(fullpath, "wb");
    if (!file) {
        sendResponse(sock, "500 Internal Server Error", "text/plain", "500 Internal Server Error");
        return;
    }

    fwrite(body, sizeof(char), strlen(body), file);
    fclose(file);

    sendResponse(sock, "201 Created", "application/json", "{\"message\": \"File created\"}");
}

void handlePut(int sock, const char *path, const char *body) {
    printf("handlePut: Socket %d - Updating file %s\n", sock, path);

    char fullpath[BUFFER_SIZE];
    snprintf(fullpath, sizeof(fullpath), "%s%s", ROOT_DIR_ACCESS, path);

    FILE *file = fopen(fullpath, "wb");
    if (!file) {
        sendResponse(sock, "500 Internal Server Error", "text/plain", "500 Internal Server Error");
        return;
    }

    fwrite(body, sizeof(char), strlen(body), file);
    fclose(file);

    sendResponse(sock, "200 OK", "application/json", "{\"message\": \"File updated\"}");
}

void handleDelete(int sock, const char *path) {
    printf("handleDelete: Socket %d - Deleting file %s\n", sock, path);

    char fullpath[BUFFER_SIZE];
    snprintf(fullpath, sizeof(fullpath), "%s%s", ROOT_DIR_ACCESS, path);

    if (remove(fullpath) == 0) {
        sendResponse(sock, "200 OK", "application/json", "{\"message\": \"File deleted\"}");
    } else {
        sendResponse(sock, "404 Not Found", "text/plain", "404 Not Found");
    }
}

