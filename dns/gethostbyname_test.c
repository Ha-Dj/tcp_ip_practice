#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: %s <host name>\n", argv[0]);
        exit(1);
    }

    struct hostent *host;
    host = gethostbyname(argv[1]);
    if (host == NULL) {
        printf("%s not found\n", argv[1]);
        exit(1);
    }
    
    printf("Offical name %s\n", host->h_name);
    for (int i = 0; host->h_aliases[i]; ++i) {
        printf("Alias %s\n", host->h_aliases[i]);
    }
    if (host->h_addrtype == AF_INET) {
        printf("Address type IPV4\n");
    } else {
        printf("Address type IPV6\n");
    }

    for (int i = 0; host->h_addr_list[i]; ++i) {
        printf("Host address %s\n", inet_ntoa(*(struct in_addr *)host->h_addr_list[i]));
    }

    return 0;
}