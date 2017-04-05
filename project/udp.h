#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


/**
 * [udpSockect create a udp socket on a random port]
 * @return [status]
 */
int udpSockect();
/**
 * [udpWriteTo write to a udp socket]
 * @param  udp_descriptor [udp_descriptor of sender]
 * @param  mensage        [message to send]
 * @param  length         [length of message]
 * @param  ip             [ip of receiver]
 * @param  port           [port of receiver]
 * @return                [status]
 */
int udpWriteTo(int udp_descriptor,char* mensage, int length,char * ip, int port);
/**
 * [udpRead read from udp]
 * @param  udp_descriptor [udp descriptor of reader]
 * @param  buffer         [buffer to send]
 * @param  length         [len of buffer]
 * @return                [status]
 */
int udpRead(int udp_descriptor, char * buffer, int length);
/**
 * [udpReadAndGetSender ]
 * @param  udp_descriptor [fd of sender]
 * @param  buffer         [buffer to send]
 * @param  length         [length to send]
 * @param  addr           [addr of receiver]
 * @return                [status]
 */
int udpReadAndGetSender(int udp_descriptor, char * buffer, int length , struct sockaddr_in ** addr,size_t  * slen);


/**
 * [udpWriteToWithSockAddr write to a udp socket with sock addr]
 * @param  udp_descriptor [fd of sender]
 * @param  mensage        [message to send]
 * @param  length         [description]
 * @param  server         [description]
 * @return                [description]
 */
int udpWriteToWithSockAddr(int udp_descriptor,char * mensage, int length,struct sockaddr_in *  server,size_t  * slen);

/**
 * [udpServer create a udp socket on a specific port]
 * @param  port [port to bind]
 * @return      [fd of socket]
 */
int udpServer(int port);
