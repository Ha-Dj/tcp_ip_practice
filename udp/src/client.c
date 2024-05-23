#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(-1);
}

#define BUF_SIZE 1024

int main() {
    int sock_fd;
    struct sockaddr_in to_addr, from_addr;
    socklen_t from_len;
    char message[BUF_SIZE];
    char recv_message[BUF_SIZE];
    sock_fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock_fd == -1) {
        error_handling("client socket create failed!");
    }

    memset(&to_addr, 0, sizeof(to_addr));
    to_addr.sin_family = AF_INET;
    to_addr.sin_addr.s_addr = INADDR_ANY;
    to_addr.sin_port = htons(9090);

    while (1) {
        ssize_t write_len = 0, read_len = 0, read_cnt = 0;
        fputs("send to server(Q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            break;
        }
        if (sendto(sock_fd, message, strlen(message), 0, (struct sockaddr *)&to_addr, sizeof(to_addr)) == -1) {
            close(sock_fd);
            perror("sendto error");
        }
        printf("here\n");
        ssize_t str_len =  recvfrom(sock_fd, recv_message, BUF_SIZE, 0, (struct sockaddr*)&from_addr, &from_len);
        if (str_len == -1) {
            close(sock_fd);
            perror("recvfrom error");
        }
        recv_message[str_len] = '\0';
        printf("receive from server: %s", message);
    }
    close(sock_fd);
    return 0;
}