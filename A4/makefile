CC = gcc
CFLAGS = -ggdb -Wall -pedantic -std=c11

all: server client

server: server.c
	gcc -o server server.c

client: client.c
	gcc -o client client.c

runs: #run server
	./server
	
runc: #run client
	./client

clean:
	rm -f *.o server client
	
