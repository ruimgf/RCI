#include <stdint.h>
#include <string.h>
/**
 * [tcpBindListen create a socket bind and listen]
 * @param  server_port [server port to bind]
 * @return             [socket fd]
 */
int tcpBindListen(int server_port);

/**
 * [tcpAccept accept new connection]
 * @param  myFd [fd to accept]
 * @return      [fd of new connection]
 */
int tcpAccept(int myFd,char * ipAccept,int * tptAccept);
/**
 * [tcpConnect make a connection]
 * @param  server_ip   [ip of server to connect]
 * @param  server_port [port of server]
 * @return             [socket of connection]
 */
int tcpConnect(char * server_ip, int server_port);
/**
 * [tcpWrite write to a TCP socket]
 * @param  tcp_descriptor [descriptor of connection]
 * @param  mensage        [message to send]
 * @param  length         [length of message]
 * @return                [status]
 */
int tcpWrite(int tcp_descriptor,char * mensage, int length);
/**
 * [tcpRead read from a socket]
 * @param  tcp_descriptor [tcp descriptor of connection]
 * @param  buffer         [buffer to save read]
 * @param  length         [length of max read]
 * @return                [status]
 */
int tcpRead(int tcp_descriptor, char * buffer, int length);
