//
// Created by lixc on 2023/4/10.
//
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>

#define    MAXLINE        4096
#define    SERV_PORT      12345

static int count;

static void recvfrom_count() {
    printf("\nreceived %d datagrams\n", count);
    exit(EXIT_SUCCESS);
}

int main() {
    int socket_fd;
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd == -1) {
        perror("socket() error");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (-1 == bind(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr))) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    socklen_t client_len;
    char message[MAXLINE];
    count = 0;

    signal(SIGINT, recvfrom_count);

    struct sockaddr_in client_addr;
    client_len = sizeof(client_addr);
    while (1) {
        ssize_t n = recvfrom(socket_fd, message, MAXLINE, 0, (struct sockaddr *) &client_addr,
                             &client_len);
        if (n == -1) {
            perror("recvfrom failed");
            exit(EXIT_FAILURE);
        }

        message[n] = 0;
        printf("received %ld bytes: %s\n", n, message);

        char send_line[MAXLINE + 4];
        sprintf(send_line, "Hi, %s", message);
        sendto(socket_fd, send_line, strlen(send_line), 0, (struct sockaddr *) &client_addr,
               client_len);
        count++;
    }
}