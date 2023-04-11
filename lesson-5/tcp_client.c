//
// Created by lixc on 2023/4/9.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MESSAGE_SIZE 102400

void send_data(int sockfd) {
    char *query;
    query = malloc(MESSAGE_SIZE + 1);
    for (int i = 0; i < MESSAGE_SIZE; i++) {
        query[i] = 'a';
    }
    query[MESSAGE_SIZE] = '\0';

    const char *cp;
    cp = query;
    size_t remaining = strlen(query);
    while(remaining) {
        ssize_t n_written = send(sockfd, cp, remaining, 0);
        fprintf(stdout, "send into buffer %ld \n", n_written);
        if(n_written <= 0) {
            perror("send failed");
            exit(EXIT_FAILURE);
        }

        remaining -= n_written;
        cp += n_written;
    }
}

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in server_addr;

    if (argc != 2) {
        perror("usage: tcp_client <address>");
        exit(EXIT_FAILURE);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
    // TODO: 错误处理
    int connect_rt = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (connect_rt == -1) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    send_data(sockfd);
    exit(0);
}