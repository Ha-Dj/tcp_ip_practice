#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "const.h"



int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: %s <output path>\n", argv[0]);
        exit(1);
    }

    char buf[BUF_SIZE];
    int sock_fd;
    struct sockaddr_in sock_addr;
    socklen_t sock_addr_len;
    int read_cnt;

    sock_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket failed\n");
        exit(1);
    }

    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr((char *)SERVER_IP);
    sock_addr.sin_port = htons(SERVER_PORT);
    
    if (connect(sock_fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) == -1) {
        perror("connect failed\n");
        exit(1);
    }

    FILE *file_fd = fopen(argv[1], "wb");
    if (file_fd == NULL) {
        perror("open file failed\n");
        exit(1);
    }

    while(1) {
        read_cnt = read(sock_fd, buf, BUF_SIZE);
        if (read_cnt < BUF_SIZE) {
            // 读不满 BUF_SIZE字节
            fwrite(buf, 1, read_cnt, file_fd);
            break;
        }
        fwrite(buf, 1, BUF_SIZE, file_fd);
    }

    printf("Received file data and stored in %s\n", argv[1]);
    write(sock_fd, "Thank you\n", 11);

    fclose(file_fd);
    close(sock_fd);
    return 0;
}