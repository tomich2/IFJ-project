######  Makefile ######

CC=gcc
CFLAGS=-std=c99 -g -Wall -Wextra -lm -O2

all: lexical

lexstring.o: lexstring.c lexstring.h
	$(CC) $(CFLAGS) -c $<

lexical.o: lexical.c lexical.h
	$(CC) $(CFLAGS) -c $<

main.o: main.c
	$(CC) $(CFLAGS) -c $<

error.o: error.c error.h
	$(CC) $(CFLAGS) -c $<

lexical: lexical.o lexstring.o main.o error.o
	$(CC) $(CFLAGS) -o $@ lexical.o lexstring.o main.o error.o
