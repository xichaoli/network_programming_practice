//
// Created by lixc on 2023/4/7.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

int make_socket(uint16_t port) {
    int sock;
    struct sockaddr_in name;

    // 创建字节流类型的IPV4 socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 绑定到port和ip
    name.sin_family = AF_INET; /* IPv4 */
    name.sin_port = htons(port); /* port */
    name.sin_addr.s_addr = htonl(INADDR_ANY); /* 通配地址 */
    // 把 IPv4地址转换成通用地址格式，同时传递长度
    if (bind(sock, (struct sockaddr *) &name, sizeof(name)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    return sock;
}

int main() {
    int sock = make_socket(12345);
    printf("The generated sock is %d. \n", sock);
    return 0;
}