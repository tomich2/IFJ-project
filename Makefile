######  Makefile ######

CC=gcc
CFLAGS=-std=c99 -g -Wall -Wextra -lm -O2

%.o: %.c
	$(CC) $(CFLAGS) -c $<

all: lexical

lexical: lexical.o lexstring.o main.o error.o stack.o parser.o rules.o
	$(CC) $(CFLAGS) -o $@ lexical.o lexstring.o main.o error.o stack.o parser.o rules.o


## ## ## ##
lexstring.o: lexstring.c lexstring.h
lexical.o: lexical.c lexical.h
error.o: error.c error.h
stack.o: stack.c stack.h
parser.o: parser.c parser.h
rules.o: rules.c rules.h
main.o: main.c
## ## ## ##
