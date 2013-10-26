all: shared
 
signal: shared.c
	gcc shared.c -o shared -Wall

debug: signal.c
	gcc shared.c -o shared -Wall -g
 
clean: shared
	rm shared
