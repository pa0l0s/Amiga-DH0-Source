#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/bsdsocket.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        PutStr("Użycie: GetHTML http://adres/\n");
        return 1;
    }

    char host[256];
    char path[512];

    // Bardzo proste parsowanie URL
    if (sscanf(argv[1], "http://%255[^/]%511[^\n]", host, path) < 1)
    {
        PutStr("Nieprawidłowy adres URL\n");
        return 1;
    }

    if (strlen(path) == 0)
        strcpy(path, "/");

    struct hostent *he = gethostbyname(host);
    if (!he)
    {
        PutStr("Nie można rozwiązać nazwy hosta\n");
        return 1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        PutStr("Nie można utworzyć gniazda\n");
        return 1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    memcpy(&addr.sin_addr, he->h_addr, he->h_length);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        PutStr("Błąd połączenia\n");
        CloseSocket(sock);
        return 1;
    }

    char request[1024];
    snprintf(request, sizeof(request),
             "GET %s HTTP/1.0\r\n"
             "Host: %s\r\n"
             "User-Agent: AmigaOS-CLI\r\n"
             "\r\n",
             path, host);

    send(sock, request, strlen(request), 0);

    char buffer[BUFFER_SIZE];
    int bytes;

    while ((bytes = recv(sock, buffer, BUFFER_SIZE - 1, 0)) > 0)
    {
        buffer[bytes] = 0;
        PutStr(buffer);
    }

    CloseSocket(sock);
    return 0;
}
