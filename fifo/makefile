all: fifo
 
fifo: fifo.c
	gcc fifo.c -o fifo -Wall

debug: signal.c
	gcc fifo.c -o fifo -Wall -g
 
clean:	fifo
	rm fifo
	rm *.fifo
	rm out.txt
