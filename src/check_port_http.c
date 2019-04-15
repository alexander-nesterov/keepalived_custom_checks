#include <stdlib.h>
#include <string.h>
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
gcc check_port_http.c -o check_port_http -Wall
gcc -D _DEBUG check_port_http.c -o check_port_http -Wall
*/

int main(int argc, char **argv)
{
    char *host = NULL;
    int port;
    char *path = NULL;
    int sock;
    int ret;
    //int code;
    struct sockaddr_in addr;
    char buf[256];

    if (argc != 4)
    {
        #ifdef _DEBUG
            printf(RED "%s\n", "Usage: <target host>:<target port>" RESET);
        #endif
        return 1;
    }

    host = argv[1];
    port = atoi(argv[2]);
    path = argv[3];

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

    ret = send(sock, path, sizeof(path), 0);
    if (ret < 0)
    {
        #ifdef _DEBUG
            printf(RED "%s\n", "Cannot send data" RESET);
        #endif
        return 1;
    }

    while(recv(sock, buf, sizeof(path), 0) > 0)
    {
        #ifdef _DEBUG
            printf("%s", buf);
        #endif
    }

    close(sock);
    return 0;
}