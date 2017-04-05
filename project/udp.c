#include "udp.h"

/**
 * [udpSockect create a udp socket on a random port]
 * @return [status]
 */
int udpSockect()
{

  int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

  if (sock_fd == -1)
    return(-1);
  return(sock_fd);
}

/**
 * [udpWriteTo write to a udp socket]
 * @param  udp_descriptor [udp_descriptor of sender]
 * @param  mensage        [message to send]
 * @param  length         [length of message]
 * @param  ip             [ip of receiver]
 * @param  port           [port of receiver]
 * @return                [status]
 */
int udpWriteTo(int udp_descriptor,char * mensage, int length,char * ip, int port)
{
    //verificar argumentos
    struct sockaddr_in server;
    size_t slen = sizeof(server);

    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    inet_aton(ip, &server.sin_addr);

    return sendto(udp_descriptor,mensage, length, 0, (struct sockaddr*) &server, slen);
}
/**
 * [udpRead read from udp]
 * @param  udp_descriptor [udp descriptor of reader]
 * @param  buffer         [buffer to send]
 * @param  length         [len of buffer]
 * @return                [status]
 */
int udpRead(int udp_descriptor, char * buffer, int length)
{
  struct sockaddr_in addr;
  socklen_t addrlen;

  addrlen = sizeof(addr);

  return recvfrom(udp_descriptor,buffer,length,0,(struct sockaddr*)&addr,&addrlen);
}

/**
 * [udpReadAndGetSender ]
 * @param  udp_descriptor [fd of sender]
 * @param  buffer         [buffer to send]
 * @param  length         [length to send]
 * @param  addr           [addr of receiver]
 * @return                [status]
 */
int udpReadAndGetSender(int udp_descriptor, char * buffer, int length , struct sockaddr_in ** addr, socklen_t  * slen){
  struct sockaddr_in * addr_aux = malloc(sizeof(struct sockaddr_in));
  if(addr_aux == NULL){
      return -1;
  }
  // testar malloc
  *addr = addr_aux;
  //socklen_t addrlen;
  *slen = sizeof(*addr_aux);

  return recvfrom(udp_descriptor,buffer,length,0,(struct sockaddr*)addr_aux,slen);
}

/**
 * [udpWriteToWithSockAddr write to a udp socket with sock addr]
 * @param  udp_descriptor [fd of sender]
 * @param  mensage        [message to send]
 * @param  length         [description]
 * @param  server         [description]
 * @return                [description]
 */
int udpWriteToWithSockAddr(int udp_descriptor,char * mensage, int length,struct sockaddr_in * server, socklen_t  * slen){

    return sendto(udp_descriptor,mensage, length, 0,(struct sockaddr *) server, *slen);

}

/**
 * [udpServer create a udp socket on a specific port]
 * @param  port [port to bind]
 * @return      [fd of socket]
 */
int udpServer(int port){
  int fd = socket(AF_INET,SOCK_DGRAM,0);
  struct sockaddr_in serveraddr;

  if(fd == -1){
    return -1;
  }

  memset((void*)&serveraddr,(int)'\0',sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((u_short)port);

  if(bind(fd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))==-1){
    return -1;
  }

  return fd;
}
