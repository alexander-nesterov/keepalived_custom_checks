#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#ifdef _DEBUG
    #include <stdio.h>
#endif

#ifdef _DEBUG
    #define RED   "\x1B[31m"
    #define GRN   "\x1B[32m"
    #define RESET "\x1B[0m"
#endif

/*
gcc check_port.c -o check_port -Wall
gcc -D _DEBUG check_port.c -o check_port -Wall
*/

int main(int argc, char **argv)
{
    char *host = NULL;
    int port;
    int sock;
    int ret;
    struct sockaddr_in addr;

    if (argc != 3)
    {
        #ifdef _DEBUG
            printf(RED "%s\n", "Usage: <target host>:<target port>" RESET);
        #endif
        return 1;
    }

    host = argv[1];
    port = atoi(argv[2]);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        #ifdef _DEBUG
            printf(RED "%s\n", "Cannot create socket" RESET);
        #endif
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, host, &addr.sin_addr);

    ret = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0)
    {
        #ifdef _DEBUG
            printf(RED "Cannot connect to host %s:%d is FAILED\n" RESET, host, port);
        #endif
        return 1;
    }

    close(sock);
    #ifdef _DEBUG
        printf(GRN "Connect to host %s:%d is OK\n" RESET, host, port);
    #endif
    return 0;
}