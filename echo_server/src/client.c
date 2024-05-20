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
    struct sockaddr_in server_addr;
    char message[BUF_SIZE];
    sock_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        error_handling("client socket create failed!");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(9090);

    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        close(sock_fd);
        error_handling("connect failed");
    }

    while (1) {
        ssize_t write_len = 0, read_len = 0, read_cnt = 0;
        fputs("send to server(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            break;
        }
        write_len = write(sock_fd, message, sizeof(message) - 1);
        while (read_len < write_len) {
            read_cnt = read(sock_fd, message + read_cnt, BUF_SIZE - 1);
            if (read_cnt == -1) {
                error_handling("read error");
            }
            read_len += read_cnt;
        }
        message[read_len] = 0;
        printf("receive from server: %s", message);
    }
    close(sock_fd);
    return 0;
}