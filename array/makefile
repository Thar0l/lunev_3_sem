CC = gcc
CFLAGS = -Wall -fprofile-arcs -ftest-coverage -g
AR = ar
ARFLAGS = rcs
all: test

test: test.o array.o
	$(CC) $(CFLAGS) test.o array.o -o test
libarray.a: array.o
	$(AR) $(ARFLAGS) libarray.a array.o
array.o: array.c array.h
	$(CC) $(CFLAGS) -c array.c
test.o: test.c array.h
	$(CC) $(CFLAGS) -c test.c

clean:
	rm *.o
	rm test