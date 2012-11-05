#Makefile
CLIENT = client.o book.o abclient.o
SERVER = server.o book.o abserver.o
CC = gcc
CFLAGS = -O -g -lstdc++

all: 	client server
client:	$(CLIENT)
	$(CC) $(CFLAGS) $(CLIENT) -o abclient
server: $(SERVER)
	$(CC) $(CFLAGS) $(SERVER) -o abserver
client.o:	client.cpp common.h abClient.h
	$(CC) $(CFLAGS) -c $< -o client.o
server.o:	server.cpp common.h abServer.h
	$(CC) $(CFLAGS) -c $< -o server.o
book.o:	book.cpp book.h common.h
	$(CC) $(CFLAGS) -c $< -o $@
abclient.o:	abClient.cpp abClient.h book.h common.h
	$(CC) $(CFLAGS) -c $< -o $@
abserver.o:	abServer.cpp abServer.h book.h common.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:	rm *.o main


