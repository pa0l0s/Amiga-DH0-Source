#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <sys/socket.h>
#include <netinet/in.h>

/* Define socklen_t if missing */
#if !defined(socklen_t)
typedef unsigned int socklen_t;
#endif

int main(int argc, char *argv[])
{
    const char *ip = "127.0.0.1";   /* default */
    int port = 5566;

    int sock;
    struct sockaddr_in addr;
    char buffer[1024];
    int r;

    /* IP from command line */
    if (argc > 1) {
        ip = argv[1];
    }

    printf("Using IP: %s\n", ip);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("[-]Socket error");
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    r = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (r < 0) {
        perror("[-]Connect error");
        close(sock);
        exit(1);
    }

    printf("[+]Connected to the server.\n");

    strcpy(buffer, "HELLO, THIS IS CLIENT.");
    printf("Client: %s\n", buffer);
    send(sock, buffer, strlen(buffer), 0);

    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer) - 1, 0);
    printf("Server: %s\n", buffer);

    close(sock);
    printf("Disconnected from the server.\n");

    return 0;
}
