CC      = gcc
CFLAGS  = -Wall -pedantic -std=c99 -O0

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble instructions emulate


assemble: assemble.o
	$(CC) $(CFLAGS) -o $@ $^
instructions.o: instructions.c instructions.h
	$(CC) $(CFLAGS) instructions.c  -c -o instructions.o
emulate.o: emulate.c instructions.h
	$(CC) $(CFLAGS) emulate.c -c -o emulate.o
emulate: emulate.o instructions.o
	$(CC) $(CFLAGS) emulate.o instructions.o -o emulate

clean:
	rm -f *.o
	rm -f assemble
	rm -f emulate
