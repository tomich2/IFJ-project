######  Makefile ######

CC=gcc
CFLAGS=-std=c99 -g -Wall -Wextra -lm -O2

all: lexical

lexstring.o: lexstring.c lexstring.h
	$(CC) $(CFLAGS) -c $<

lexical.o: lexical.c lexical.h
	$(CC) $(CFLAGS) -c $<

lexical: lexical.o lexstring.o
	$(CC) $(CFLAGS) -o $@ lexical.o lexstring.o
