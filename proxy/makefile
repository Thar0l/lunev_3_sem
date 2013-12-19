all: proxy
 
proxy: proxy.c
	g++ proxy.cpp -o proxy -Wall

debug: signal.c
	g++ proxy.cpp -o proxy -Wall -g
 
clean:	proxy
	rm proxy
