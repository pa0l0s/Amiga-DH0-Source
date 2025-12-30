#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <proto/exec.h>
#include <proto/socket.h>

#if !defined(socklen_t)
typedef unsigned int socklen_t;
#endif

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("Usage: %s <IP_ADDRESS>\n", argv[0]);
        return 1;
    }

    char *server_ip = argv[1];
    int port = 80; // HTTP standard

    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Tworzymy socket TCP
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        perror("socket");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    // Łączenie z serwerem
    if(connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sock);
        return 1;
    }

    // Wysyłamy prosty request HTTP/1.0
    const char *http_request = "GET / HTTP/1.0\r\nHost: ";
    send(sock, http_request, strlen(http_request), 0);
    send(sock, server_ip, strlen(server_ip), 0);
    send(sock, "\r\n\r\n", 4, 0);

    // Odbieranie i wypisywanie odpowiedzi
    int n;
    while((n = recv(sock, buffer, BUFFER_SIZE-1, 0)) > 0) {
        buffer[n] = 0; // null-terminator
        printf("%s", buffer);
    }

    close(sock);
    return 0;
}
