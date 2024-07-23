#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

void sendResponse(int sock, const char *status, const char *content_type, const char *body);
const char *getContentType(const char *path);
void serveFile(int sock, const char *path);
void listDirectory(int sock, const char *path);
void handleRead(int sock, const char *path);
void handleHeader(int sock, const char *path);
void handlePost(int sock, const char *path, const char *body);
void handlePut(int sock, const char *path, const char *body);
void handleDelete(int sock, const char *path);

#endif