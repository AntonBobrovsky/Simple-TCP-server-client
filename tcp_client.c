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

    int sock;
    char buffer[BUF_LEN];
    struct sockaddr_in clnt_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        exit(-1);
    }

    bzero(&clnt_addr, sizeof(clnt_addr));
    clnt_addr.sin_family = AF_INET;
    clnt_addr.sin_port = htons(3000);
    clnt_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *) &clnt_addr, sizeof(clnt_addr))) {
        perror("connect()");
        exit(-1);
    }

    while (1) {
        printf("Input:");
        fgets(buffer, BUF_LEN, stdin);

        if ((strncmp(buffer, "quit", 4)) == 0) {
            close(sock);
            exit(0);
        }

        if (send(sock, buffer, BUF_LEN, 0) < 0) {
            perror("send()");
            close(sock);
            exit(-1);
        }

        if (recv(sock, buffer, BUF_LEN, 0) < 0) {
            perror("recv()");
            close(sock);
            exit(-1);
        }

        printf("Got: %s", buffer);
    }

    close(sock);
    return 0;
}
