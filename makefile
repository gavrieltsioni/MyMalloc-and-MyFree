CFLAGS=-Wall -g -o
CC=gcc
clean:
	rm malloc

test1:	
	$(CC) $(CFLAGS) malloc malloc.c test1.c

test2:
	$(CC) $(CFLAGS) malloc malloc.c test2.c	

test3:
	$(CC) $(CFLAGS) malloc malloc.c test3.c

test4:
	$(CC) $(CFLAGS) malloc malloc.c test4.c
