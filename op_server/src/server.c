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

int calculate(int cnt, int opnds[], char operator) {
    if (cnt < 1) {
        return 0;
    }
    for (int i = 0; i < cnt; ++i) {
        opnds[i] = ntohl(opnds[i]);
    }
    int result = 0;
    switch (operator)
    {
    case '+':
        for (int i = 0; i < cnt; ++i) {
            result += opnds[i];
        }
        break;
    
    case '-':
        result = opnds[0];
        for (int i = 1; i < cnt; ++i) {
            result -= opnds[i];
        }
        break;
    
    case '*':
        result = opnds[0];
        for (int i = 1; i < cnt; ++i) {
            result *= opnds[i];
        }
        break;
    default:
        perror("unkonwn operator\n");
        return -1;
    }
    return result;
}

int main() {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_len;

    char opinfo[BUF_SIZE];
    
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        error_handling("socket error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr((char *)SERVER_ADDR);
    serv_addr.sin_port = htons(SERVER_PORT);
    
    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind error");
    }

    if (listen(serv_sock, 5) == -1) {
        error_handling("listen error");
    }
    
    for (int i = 0; i < 5; ++i) {
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len);
        if (clnt_sock == -1) {
            close(clnt_sock);
            perror("accept return -1\n");
            continue;
        }

        int opnd_cnt = 0;
        ssize_t read_cnt = 0;
        ssize_t read_len = 0;
        // 读取第一个 cnt
        if (read(clnt_sock, &opnd_cnt, 1) == -1) {
           close(clnt_sock);
           perror("read return -1\n");
           continue;
        }
        // 读取后续 opnd，放入 opinfo数组中
        read_len = 0;
        read_cnt = 0;
        while (opnd_cnt * OPSZ + 1 > read_len) {
            read_cnt = read(clnt_sock, &opinfo[read_len], BUF_SIZE - 1);
            read_len += read_cnt;
        }
        
        int result = htonl(calculate(opnd_cnt, (int *)opinfo, opinfo[read_len - 1]));
        if (write(clnt_sock, (char *)&result, sizeof(result)) == -1) {
            perror("write error");
            close(clnt_sock);
            continue;
        }
        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}