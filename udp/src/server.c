#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(-1);
}

#define BUF_SIZE 1024

int main() {
    char message[BUF_SIZE];
    // create sock
    int sock_fd;
    struct sockaddr_in sock_addr, from_addr;
    socklen_t from_len;
    sock_fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock_fd == -1) {
        error_handling("server socket create failed!");
    }

    // bind
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = INADDR_ANY;
    sock_addr.sin_port = htons(9090);

    if (bind(sock_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) == -1) {
        close(sock_fd);
        error_handling("server bind failed");
    }

    // accept
    for (int i = 0; i < 2; ++i) {
        ssize_t recvlen = recvfrom(sock_fd, message, sizeof(message), 0, (struct sockaddr*)&from_addr, &from_len);
        if (recvlen == -1) {
            continue;
        }
        sendto(sock_fd, message, recvlen, 0, (struct sockaddr*)&from_addr, from_len);
    }
    close(sock_fd);
    return 0;
}