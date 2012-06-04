CC      = gcc
CFLAGS  = -Wall -pedantic -std=c99 -g -O0

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o
	$(CC) $(CFLAGS) -o $@ $^

emulate: emulate.o instructions.o utils.o
	$(CC) $(CFLAGS) -o $@ $^

test_tokeniser: test_tokeniser.o tokeniser.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o
	rm -f assemble
	rm -f emulate
