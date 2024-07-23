#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../include/utils.h"
#include "http_handler.h"
#include "request_router.h"

#define BUFFER_SIZE 4096
#define TEST_DIR "www/api/test_dir"
#define TEST_FILE "www/api/test_file.txt"

#define TESTING

ssize_t read_all(int sockfd, char *buffer, size_t size) {
    ssize_t total_bytes_read = 0;
    ssize_t bytes_read;
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    while ((bytes_read = read(sockfd, buffer + total_bytes_read, size - total_bytes_read)) > 0) {
        total_bytes_read += bytes_read;
    }

    if (bytes_read == -1 && errno == EAGAIN) {
        std::cout << "Socket read timed out" << std::endl;
    }

    close(sockfd);

    return total_bytes_read;
}

void setup_test_environment() {
    std::cout << "Setting up test environment..." << std::endl;
    mkdir(TEST_DIR, 0755);
    FILE *file = fopen(TEST_FILE, "w");
    if (file) {
        fprintf(file, "Hello, world!");
        fclose(file);
    }
}

void cleanup_test_environment() {
    std::cout << "Cleaning up test environment..." << std::endl;
    remove(TEST_FILE);
    rmdir(TEST_DIR);
}

Test(Basic, SimpleTest) {
    std::cout << "Running SimpleTest..." << std::endl;
    cr_assert(1 == 1, "1 should equal 1");
    std::cout << "Finished SimpleTest..." << std::endl;
}

Test(Server, HandleRead) {
    std::cout << "Running HandleRead test..." << std::endl;

    setup_test_environment();

    int sockfd[2];
    cr_assert(socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd) == 0, "Failed to create socket pair");

    routeRequest(sockfd[0], "GET", "/test_file.txt", NULL);

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read_all(sockfd[1], buffer, sizeof(buffer));

    std::cout << "Bytes read: " << bytes_read << std::endl;
    std::cout << "Buffer content: " << std::string(buffer, bytes_read) << std::endl;

    close(sockfd[0]);

    const char *expected_response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 13\r\n"
        "Connection: close\r\n"
        "\r\n"
        "Hello, world!";

    cr_assert_eq(bytes_read, (ssize_t)strlen(expected_response), "Expected %zu bytes but got %zu bytes",
                 strlen(expected_response), bytes_read);
    cr_assert(memcmp(buffer, expected_response, bytes_read) == 0, "Response does not match expected value");

    cleanup_test_environment();
}

Test(Server, HandleHeader) {
    std::cout << "Running HandleHeader test..." << std::endl;

    setup_test_environment();

    int sockfd[2];
    cr_assert(socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd) == 0, "Failed to create socket pair");

    routeRequest(sockfd[0], "HEAD", "/data.json", NULL);

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read_all(sockfd[1], buffer, sizeof(buffer));

    std::cout << "Bytes read: " << bytes_read << std::endl;
    std::cout << "Buffer content: " << std::string(buffer, bytes_read) << std::endl;

    close(sockfd[0]);

    const char *expected_response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: 19\r\n"
        "Connection: close\r\n\r\n";

    cr_assert_eq(bytes_read, (ssize_t)strlen(expected_response), "Expected %zu bytes but got %zu bytes",
                 strlen(expected_response), bytes_read);
    cr_assert(memcmp(buffer, expected_response, bytes_read) == 0, "Response does not match expected value");

    cleanup_test_environment();
}

Test(Server, HandlePost) {
    std::cout << "Running HandlePost test..." << std::endl;

    int sockfd[2];
    cr_assert(socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd) == 0, "Failed to create socket pair");

    routeRequest(sockfd[0], "POST", "/test_post.txt", "Test data");

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read_all(sockfd[1], buffer, sizeof(buffer));

    std::cout << "Bytes read: " << bytes_read << std::endl;
    std::cout << "Buffer content: " << std::string(buffer, bytes_read) << std::endl;

    close(sockfd[0]);

    const char *expected_response =
        "HTTP/1.1 201 Created\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: 27\r\n"
        "Connection: close\r\n"
        "\r\n"
        "{\"message\": \"File created\"}";

    cr_assert_eq(bytes_read, (ssize_t)strlen(expected_response), "Expected %zu bytes but got %zu bytes",
                 strlen(expected_response), bytes_read);
    cr_assert(memcmp(buffer, expected_response, bytes_read) == 0, "Response does not match expected value");

    FILE *file = fopen("www/api/test_post.txt", "r");
    cr_assert_not_null(file, "File was not created");
    char file_content[BUFFER_SIZE];
    size_t file_size = fread(file_content, 1, sizeof(file_content), file);
    file_content[file_size] = '\0';
    cr_assert_str_eq(file_content, "Test data", "File content does not match expected data");
    fclose(file);

    remove("www/api/test_post.txt");
}

Test(Server, HandlePut) {
    std::cout << "Running HandlePut test..." << std::endl;

    FILE *file = fopen("www/api/test_put.txt", "w");
    if (file) {
        fprintf(file, "Old data");
        fclose(file);
    }

    int sockfd[2];
    cr_assert(socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd) == 0, "Failed to create socket pair");

    routeRequest(sockfd[0], "PUT", "/test_put.txt", "New data");

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read_all(sockfd[1], buffer, sizeof(buffer));

    std::cout << "Bytes read: " << bytes_read << std::endl;
    std::cout << "Buffer content: " << std::string(buffer, bytes_read) << std::endl;

    close(sockfd[0]);

    const char *expected_response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: 27\r\n"
        "Connection: close\r\n"
        "\r\n"
        "{\"message\": \"File updated\"}";

    cr_assert_eq(bytes_read, (ssize_t)strlen(expected_response), "Expected %zu bytes but got %zu bytes",
                 strlen(expected_response), bytes_read);
    cr_assert(memcmp(buffer, expected_response, bytes_read) == 0, "Response does not match expected value");

    file = fopen("www/api/test_put.txt", "r");
    cr_assert_not_null(file, "File was not updated");
    char file_content[BUFFER_SIZE];
    size_t file_size = fread(file_content, 1, sizeof(file_content), file);
    file_content[file_size] = '\0';
    cr_assert_str_eq(file_content, "New data", "File content does not match expected data");
    fclose(file);

    remove("www/api/test_put.txt");
}

Test(Server, HandleDelete) {
    std::cout << "Running HandleDelete test..." << std::endl;

    FILE *file = fopen("www/api/test_delete.txt", "w");
    if (file) {
        fprintf(file, "Data to be deleted");
        fclose(file);
    }

    int sockfd[2];
    cr_assert(socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd) == 0, "Failed to create socket pair");

    routeRequest(sockfd[0], "DELETE", "/test_delete.txt", NULL);

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read_all(sockfd[1], buffer, sizeof(buffer));

    std::cout << "Bytes read: " << bytes_read << std::endl;
    std::cout << "Buffer content: " << std::string(buffer, bytes_read) << std::endl;

    close(sockfd[0]);

    const char *expected_response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: 27\r\n"
        "Connection: close\r\n"
        "\r\n"
        "{\"message\": \"File deleted\"}";

    std::cout << "Expected response: " << expected_response << std::endl;

    cr_assert_eq(bytes_read, (ssize_t)strlen(expected_response), "Expected %zu bytes but got %zu bytes",
                 strlen(expected_response), bytes_read);
    cr_assert(memcmp(buffer, expected_response, bytes_read) == 0, "Response does not match expected value");

    cr_assert(access("www/api/test_delete.txt", F_OK) == -1, "File was not deleted");
}
