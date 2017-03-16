CC = gcc
CFLAGS  = -Wall
TARGET1 = msgserv
TARGET2 = rmb
TARGET = $(TARGET1) $(TARGET2)
DEPS = udp.o messagelist.o

all: $(TARGET)

messageList.o : messagelist.c messagelist.h
		$(CC) $(CFLAGS) -c messagelist.c

tcp.o: tcp.c tcp.h
	$(CC) $(CFLAGS) -c tcp.c

udp.o: udp.c udp.h
	$(CC) $(CFLAGS) -c udp.c

$(TARGET1): $(TARGET1).c $(DEPS)
	$(CC) $(CFLAGS) $(TARGET1).c $(DEPS) -o $(TARGET1)

$(TARGET2): $(TARGET2).c $(DEPS)
	$(CC) $(CFLAGS) $(TARGET2).c $(DEPS) -o $(TARGET2)

clean:
	$(RM) *.o *~
