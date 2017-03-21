#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int udpSockect();

int udpWriteTo(int udp_descriptor,char* mensage, int length,char * ip, int port);

int udpRead(int udp_descriptor, char * buffer, int length);

int udpReadAndGetSender(int udp_descriptor, char * buffer, int length , struct sockaddr_in ** addr);

int udpWriteToWithSockAddr(int udp_descriptor,char * mensage, int length,struct sockaddr_in server);

int udpServer(int port);
