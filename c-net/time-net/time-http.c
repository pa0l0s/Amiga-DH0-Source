#include <proto/exec.h>
//#include <proto/socket.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

int main() {
	int sock;
	struct sockaddr_in server_addr;
    struct Library *SocketBase = OpenLibrary("bsdsocket.library", 4);
    if (!SocketBase) {
        printf("Failed to open bsdsocket.library\n");
        return 1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Failed to create socket\n");
        CloseLibrary(SocketBase);
        return 1;
    }
	
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    server_addr.sin_addr.s_addr = inet_addr("162.159.200.1"); // worldtimeapi.org IP

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Failed to connect\n");
        CloseSocket(sock);
        CloseLibrary(SocketBase);
        return 1;
    }

    char request[] = "GET /api/ip HTTP/1.1\r\nHost: worldtimeapi.org\r\n\r\n";
    if (send(sock, request, strlen(request), 0) < 0) {
        printf("Failed to send HTTP request\n");
        CloseSocket(sock);
        CloseLibrary(SocketBase);
        return 1;
    }

    char buffer[1024];
    int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received < 0) {
        printf("Failed to receive HTTP response\n");
        CloseSocket(sock);
        CloseLibrary(SocketBase);
        return 1;
    }

    buffer[bytes_received] = '\0';
    printf("HTTP Response:\n%s\n", buffer); // Parse JSON for datetime

    CloseSocket(sock);
    CloseLibrary(SocketBase);
    return 0;
}