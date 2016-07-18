#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define BUF_LEN 1000

int main(int argc, char const *argv[]) {

    int sock, newsock;
    int bytes;
    char buffer[BUF_LEN];
    struct sockaddr_in srv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        exit(-1);
    }

    bzero(&srv_addr, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(3000);
    srv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sock, (struct sockaddr *) &srv_addr, sizeof(srv_addr)) < 0) {
        perror("bind()");
        close(sock);
        exit(-1);
    }

    if (listen(sock, 1) < 0) {
        perror("listen()");
        close(sock);
        exit(-1);
    }

    if ((newsock = accept(sock, NULL, NULL)) < 0) {
        perror("accept()");
        close(sock);
        exit(-1);
    }

    while (1) {
        if ((bytes = recv(newsock, buffer, BUF_LEN, 0)) < 0) {
            perror("recv()");
            close(sock);
            close(newsock);
            exit(-1);
        }

        if (bytes == 0) break;

        printf("\nfrom client: %s\n", buffer);

        if (send(newsock, buffer, BUF_LEN, 0) < 0) {
            perror("send()");
            close(sock);
            close(newsock);
            exit(-1);
        }
    }

    close(sock);
    close(newsock);
    return 0;
}
