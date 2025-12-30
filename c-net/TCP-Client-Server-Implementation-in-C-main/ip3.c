#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* AmiTCP compatibility */
#ifndef socklen_t
typedef unsigned int socklen_t;
#endif

int main(void)
{
    int sock;
    struct sockaddr_in remote, local;
    socklen_t len;

    len = sizeof(local);

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("socket");
        return 1;
    }

    remote.sin_family = AF_INET;
    remote.sin_port = htons(53);
    remote.sin_addr.s_addr = inet_addr("8.8.8.8");

    connect(sock, (struct sockaddr *)&remote, sizeof(remote));
    getsockname(sock, (struct sockaddr *)&local, &len);

    printf("Local IP: %s\n", inet_ntoa(local.sin_addr));

    close(sock);
    return 0;
}
