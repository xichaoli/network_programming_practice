//
// Created by lixc on 2023/4/10.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAXLINE 4096
#define SERV_PORT 12345

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <IP> \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int socket_fd;
    socket_fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (socket_fd == -1) {
        perror("socket() error");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

    socklen_t serv_len = sizeof(server_addr);
    struct sockaddr *reply_addr;
    reply_addr = malloc(serv_len);

    char send_line[MAXLINE], recv_line[MAXLINE + 1];
    socklen_t len;
    ssize_t n;

    while (fgets(send_line, MAXLINE, stdin) != NULL) {
        size_t i = strlen(send_line);
        if (send_line[i - 1] == '\n') {
            send_line[i - 1] = 0;
        }

        printf("now sending %s\n", send_line);
        size_t rt = sendto(socket_fd, send_line, strlen(send_line), 0, (struct sockaddr *) &server_addr, serv_len);
        if (rt == -1) {
            perror("send failed");
            exit(EXIT_FAILURE);
        }
        printf("send bytes: %zu \n", rt);

        len = 0;
        n = recvfrom(socket_fd, recv_line, MAXLINE, 0, reply_addr, &len);
        if (n == -1) {
            perror("recvfrom failed");
            exit(EXIT_FAILURE);
        }
        recv_line[n] = 0;
        fputs(recv_line, stdout);
        fputs("\n", stdout);
    }

    close(socket_fd);
    return 0;
}