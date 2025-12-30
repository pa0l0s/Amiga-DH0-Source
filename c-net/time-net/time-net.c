#include <proto/exec.h>
#include <stdio.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define NTP_SERVER "pool.ntp.org"
#define NTP_PORT 123
#define NTP_TIMESTAMP_DELTA 2208988800UL // Seconds between 1900 and 1970

typedef unsigned short uint8_t;
typedef unsigned long uint32_t;

struct ntp_packet {
    uint8_t li_vn_mode;
    uint8_t stratum;
    uint8_t poll;
    uint8_t precision;
    uint32_t root_delay;
    uint32_t root_dispersion;
    uint32_t ref_id;
    uint32_t ref_timestamp_sec;
    uint32_t ref_timestamp_frac;
    uint32_t orig_timestamp_sec;
    uint32_t orig_timestamp_frac;
    uint32_t recv_timestamp_sec;
    uint32_t recv_timestamp_frac;
    uint32_t trans_timestamp_sec;
    uint32_t trans_timestamp_frac;
};

int main() {
	int sock;
	struct sockaddr_in server_addr;
    // Set up NTP packet (first byte: 0x1B = LI=0, VN=3, Mode=3)
    struct ntp_packet packet = { 0x1B, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	fd_set read_fds;
	struct timeval timeout = { 5, 0 }; // 5 sec timeout
	uint32_t ntp_time;
	time_t current_time;
	
    struct Library *SocketBase = OpenLibrary("bsdsocket.library", 4);
    if (!SocketBase) {
        printf("Failed to open bsdsocket.library\n");
        return 1;
    }

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create socket\n");
        CloseLibrary(SocketBase);
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(NTP_PORT);
    server_addr.sin_addr.s_addr = inet_addr("216.239.35.12"); // NTP server IP 162.159.200.1

    if (sendto(sock, &packet, sizeof(packet), 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Failed to send NTP request\n");
        close(sock);
        CloseLibrary(SocketBase);
        return 1;
    }

    // Wait for response
    FD_ZERO(&read_fds);
    FD_SET(sock, &read_fds);

    if (select(sock + 1, &read_fds, NULL, NULL, &timeout) <= 0) {
        printf("NTP timeout\n");
        close(sock);
        CloseLibrary(SocketBase);
        return 1;
    }

    // Receive NTP response
    if (recv(sock, &packet, sizeof(packet), 0) < 0) {
        printf("Failed to receive NTP response\n");
        close(sock);
        CloseLibrary(SocketBase);
        return 1;
    }

    // Extract NTP time (convert from 1900 to 1970 epoch)
    ntp_time = ntohl(packet.trans_timestamp_sec) - NTP_TIMESTAMP_DELTA;
    current_time = (time_t)ntp_time;

    printf("Current time: %s", ctime(&current_time));

    close(sock);
    CloseLibrary(SocketBase);
    return 0;
}