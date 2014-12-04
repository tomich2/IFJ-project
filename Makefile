######  Makefile ######

CC=gcc
CFLAGS=-std=c99 -g -Wall -Wextra -lm 

%.o: %.c
	$(CC) $(CFLAGS) -c $<

all: parser

parser: lexical.o lexstring.o main.o error.o parser.o rules.o stack.o expr.o ial.o garbage.o list.o generator.o
	$(CC) $(CFLAGS) -o $@ lexical.o lexstring.o main.o error.o parser.o rules.o stack.o expr.o ial.o garbage.o list.o generator.o


## ## ## ##
lexstring.o: lexstring.c lexstring.h
lexical.o: lexical.c lexical.h
error.o: error.c error.h
parser.o: parser.c parser.h
ial.o: ial.c ial.h
rules.o: rules.c rules.h
stack.o: stack.c stack.h
expr.o: expr.c expr.h
list.o: list.c list.h
generator.o: generator.c generator.h struct.h
main.o: main.c
garbage.o: garbage.c garbage.h
## ## ## ##

clean:
	rm -rf *.o parser
