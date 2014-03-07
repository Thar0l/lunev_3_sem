CC = gcc
CFLAGS = -Wall -g3
NA = -fprofile-arcs -ftest-coverage -g
AR = ar
ARFLAGS = rcs
all: test

test: test.c libarray.a
	$(CC) $(CFLAGS) -static test.c -L. -larray -o test
libarray.a: array.o
	$(AR) $(ARFLAGS) libarray.a array.o
array.o: array.c array.h
	$(CC) $(CFLAGS) -c array.c

clean:
	rm *.o
	rm test