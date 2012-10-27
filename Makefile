#Makefile
OBJS = ab.o book.o abcli.o
CC = gcc
CFLAGS = -O -g -lstdc++

ab:	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o ab
ab.o:	ab.cpp common.h abcli.h
	$(CC) $(CFLAGS) -c $< -o ab.o
book.o:	book.cpp book.h common.h
	$(CC) $(CFLAGS) -c $< -o $@
abcli.o:	abcli.cpp abcli.h book.h common.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:	rm *.o main


