CC      = gcc
CFLAGS  = -Wall -pedantic -std=c99 

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o
	$(CC) $(CFLAGS) -o $@ $^

emulate: emulate.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o
	rm -f assemble
	rm -f emulate
