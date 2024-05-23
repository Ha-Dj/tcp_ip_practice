#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "const.h"



int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input path>\n", argv[0]);
        exit(1);
    }
    char buf[BUF_SIZE];
    int serv_fd, clnt_fd;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_len;
    int read_cnt;

    serv_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_fd == -1) {
        perror("socket failed\n");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr((char *)SERVER_IP);
    serv_addr.sin_port = htons(SERVER_PORT);
    
    if (bind(serv_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("bind failed\n");
        exit(1);
    }
    
    if (listen(serv_fd, 5) == -1) {
        perror("listen failed\n");
        exit(1);
    }

    FILE *file_fd = fopen(argv[1], "rb");
    if (file_fd == NULL) {
        perror("open file failed\n");
        exit(1);
    }

    clnt_fd = accept(serv_fd, (struct sockaddr *)&clnt_addr, &clnt_len);
    if (clnt_fd == -1) {
        perror("accpet failed\n");
        exit(1);
    }

    while(1) {
        read_cnt = fread((void *)buf, 1, BUF_SIZE, file_fd);
        if (read_cnt < BUF_SIZE) {
            // 读不满 BUF_SIZE字节
            write(clnt_fd, buf, read_cnt);
            break;
        }
        write(clnt_fd, buf, BUF_SIZE);
    }

    shutdown(clnt_fd, SHUT_WR);
    read(clnt_fd, buf, BUF_SIZE);
    printf("client reply: %s", buf);

    fclose(file_fd);
    close(clnt_fd);
    close(serv_fd);
    return 0;
}