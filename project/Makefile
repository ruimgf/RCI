CC = gcc
CFLAGS  = -Wall
TARGET = msgserv
DEPS = udp.o

all: $(TARGET)

tcp.o: tcp.c tcp.h
	$(CC) $(CFLAGS) -c tcp.c
udp.o: udp.c udp.h
	$(CC) $(CFLAGS) -c udp.c

$(TARGET): $(TARGET).c $(DEPS)
	$(CC) $(CFLAGS) $(TARGET).c $(DEPS) -o $(TARGET)

clean:
	$(RM) *.o *~
