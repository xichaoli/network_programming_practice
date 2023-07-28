#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 8192

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <multicast_address> <port>\n", argv[0]);
        exit(1);
    }

    int sock_fd;
    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    char *end_ptr;
    long port = strtol(argv[2], &end_ptr, 10);
    if (*end_ptr != '\0') {
        fprintf(stderr, "Invalid port number: %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }
    addr.sin_port = htons(port);

    if (inet_pton(AF_INET, argv[1], &addr.sin_addr) != 1) {
        fprintf(stderr, "Invalid multicast address: %s\n", argv[1]);
        exit(1);
    }

    char buf[BUF_SIZE];
    memset(buf, 0, sizeof(buf));
    snprintf(buf, sizeof(buf), "Hello, multicast!");

    /* IP_ADD_MEMBERSHIP选项用于将套接字加入到指定的多播组中 */
    struct ip_mreq mreq;
    memset(&mreq, 0, sizeof(mreq));
    mreq.imr_multiaddr.s_addr = addr.sin_addr.s_addr;
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sock_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        perror("setsockopt");
        exit(1);
    }

    /* 用于设置多播数据包的生存时间（TTL）*/
    int ttl = 5;
    if (setsockopt(sock_fd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) < 0) {
        perror("setsockopt");
        exit(1);
    }

    while (1) {
        if (sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            perror("sendto");
            exit(1);
        }
        sleep(1);
    }
}
