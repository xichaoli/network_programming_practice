//
// Created by lixc on 2023/4/8.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>

/* 从socket中读取size个字节 */
size_t readn(int fd, char *buf, size_t size) {
    char *buffer_pointer = buf;
    size_t length = size;

    while (length > 0) {
        ssize_t result = read(fd, buffer_pointer, length);
        if (result == -1) {
            if (errno == EINTR) {
                /* 考虑非阻塞的情况，这里需要再次调用read */
                continue;
            } else {
                return -1;
            }
        } else if (result == 0) {
            /* EOF(End of File)表示套接字关闭 */
            break;
        }

        length -= result;
        buffer_pointer += result;
    }

    /* 返回实际读取的字节数 */
    return (size - length);
}

void read_date(int sockfd) {
    char buf[1024];
    int times = 0;

    for(;;) {
        fprintf(stdout, "block in read\n");
        if(readn(sockfd, buf, sizeof(buf)) == 0) {
            return;
        }

        times++;
        fprintf(stdout, "1024 bytes read for %d \n", times);
        usleep(1000);
    }
}

int main() {
    int listen_fd, connect_fd;
    socklen_t client_len;
    struct sockaddr_in server_addr, client_addr;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    /* 第二个参数 backlog，在 Linux 中表示已完成 (ESTABLISHED) 且未 accept 的队列大小，
     * 这个参数的大小决定了可以接收的并发数目 */
    if (listen(listen_fd, 1024) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /* 循环处理用户请求 */
    for (;;) {
        client_len = sizeof (client_addr);
        connect_fd = accept(listen_fd, (struct sockaddr*) &client_addr, &client_len);
        if (connect_fd == -1) {
            perror("connect");
            exit(EXIT_FAILURE);
        }
        /* 读取数据 */
        read_date(connect_fd);
        /* 关闭 连接套接字 */
        close(connect_fd);
    }
}