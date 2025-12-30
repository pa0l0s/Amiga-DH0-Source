TOOL++ = g++
TOOL = gcc

.PHONY: all

all: server.c client.c
	$(TOOL) -g -Wall -o server.out server.c
	$(TOOL) -g -Wall -o client.out client.c
	$(TOOL) -o server server.out -lamiga
	$(TOOL) -o client client.out -lamiga

.PHONY: server

server: server.out
	$(TOOL) -o server server.out -lamiga

server.out: server.c
	$(TOOL) -g -Wall -o server.out server.c

.PHONY: client

server: client.out
	$(TOOL) -o client client.out -lamiga
	
client.out: client.c
	$(TOOL) -g -Wall -o client.out client.c