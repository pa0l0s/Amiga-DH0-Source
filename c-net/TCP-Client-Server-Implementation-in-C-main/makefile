TOOL = gcc
CFLAGS = -g -Wall
LIBS = -lamiga

.PHONY: all server client client2 ip3 clean

all: server client client2 ip3

server: server.c
	$(TOOL) $(CFLAGS) server.c -o server $(LIBS)

client: client.c
	$(TOOL) $(CFLAGS) client.c -o client $(LIBS)

client2: client2.c
	$(TOOL) $(CFLAGS) client2.c -o client2 $(LIBS)

ip: ip.c
	$(TOOL) $(CFLAGS) ip.c -o ip $(LIBS)

ip2: ip2.c
	$(TOOL) $(CFLAGS) ip2.c -o ip2 $(LIBS)

ip2: ip3.c
	$(TOOL) $(CFLAGS) ip3.c -o ip3 $(LIBS)

clean:
	delete server client client2 ip ip2 ip3