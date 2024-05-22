#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define RET_SIZE 4
#define OPSZ 4
#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 9090

void error_handling(const char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(-1);
}

int main() {
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("socket create error");
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr) <= 0) {
        close(sock);
        error_handling("Invalid address/ Address not supported");
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        close(sock);
        error_handling("connect error");
    } else {
        puts("Connected....");
    }

    // 生成传输协议
    char opmsg[BUF_SIZE];
    fputs("Operand count: ", stdout);
    int opnd_cnt = 0;
    scanf("%d", &opnd_cnt);
    opmsg[0] = (char)opnd_cnt;

    for (int i = 0; i < opnd_cnt; ++i) {
        int operand;
        printf("Operand %d: ", i + 1);
        scanf("%d", &operand);
        *((int *)(opmsg + 1 + i * OPSZ)) = htonl(operand);
    }
    fgetc(stdin);  // 清除缓冲区中的换行符
    fputs("Operator: ", stdout);
    scanf("%c", &opmsg[1 + opnd_cnt * OPSZ]);

    // 写入
    if (write(sock, opmsg, 2 + opnd_cnt * OPSZ) == -1) {
        close(sock);
        error_handling("write error");
    }

    // 读取结果
    int result;
    ssize_t read_cnt = 0, read_len = 0;
    while (read_len < RET_SIZE) {
        read_cnt = read(sock, &result, RET_SIZE);
        if (read_cnt == -1) {
            close(sock);
            error_handling("read error");
        }
        read_len += read_cnt;
    }
    result = ntohl(result);

    printf("Operation result: %d\n", result);

    close(sock);
    return 0;
}
