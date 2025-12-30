#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>   /* MUSI BYĆ PRZED netdb.h */
#include <arpa/inet.h>
#include <netdb.h>

int main(void)
{
    char hostname[256];
    struct hostent *he;
    struct in_addr *addr;
    int i;

    if (gethostname(hostname, sizeof(hostname)) != 0)
    {
        perror("gethostname");
        return 1;
    }

    printf("Hostname: %s\n", hostname);

    he = gethostbyname(hostname);
    if (!he)
    {
        herror("gethostbyname");
        return 1;
    }

    for (i = 0; he->h_addr_list[i] != NULL; i++)
    {
        addr = (struct in_addr *)he->h_addr_list[i];
        printf("IP: %s\n", inet_ntoa(*addr));
    }

    return 0;
}
