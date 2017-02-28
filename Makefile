tcp.o: tcp.c tcp.h
	gcc -c tcp.c -g -o tcp.o
udp.o: udp.c udp.h
	gcc -c udp.c -g -o udp.o
msgserv:
	gcc msgserv.c udp.o -o msgserv.o
