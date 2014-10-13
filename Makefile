######  Makefile ######

CC=gcc
CFLAGS=-std=c99 -g -Wall -Wextra -lm

all: projifj

main.o: main.c
	$(CC) $(CFLAGS) -c main.h $<

projifj: main.o
	$(CC) $(CFLAGS) -o $@ main.o
