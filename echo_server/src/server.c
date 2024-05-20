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
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    server_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        error_handling("server socket create failed!");
    }

    // bind
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(9090);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        close(server_fd);
        error_handling("server bind failed");
    }

    // listen
    if (listen(server_fd, 5) == -1) {
        close(server_fd);
        error_handling("server listen failed");
    }

    // accept
    for (int i = 0; i < 5; ++i) {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd == -1) {
            close(client_fd);
            close(server_fd);
            error_handling("accept error");
        }
        ssize_t read_len = 0;
        while ((read_len = read(client_fd, message, BUF_SIZE)) != 0 && read_len != -1) {
            write(client_fd, message, read_len);
        }
        close(client_fd);
    }
    close(server_fd);
    return 0;
}