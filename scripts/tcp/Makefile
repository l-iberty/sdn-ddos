.PHONY: all

all: client server

client: client.c
	gcc $< -o $@

server: server.c
	gcc $< -lpthread -o $@
