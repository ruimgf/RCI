#include "udp.h"

int udpSockect()
{

  int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

  if (sock_fd == -1)
    return(-1);
  return(sock_fd);
}


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

int udpRead(int udp_descriptor, char * buffer, int length)
{
  struct sockaddr_in addr;
  socklen_t addrlen;

  addrlen = sizeof(addr);

  return recvfrom(udp_descriptor,buffer,length,0,(struct sockaddr*)&addr,&addrlen);
}


int udpReadAndGetSender(int udp_descriptor, char * buffer, int length , struct sockaddr_in ** addr){
  struct sockaddr_in * addr_aux = malloc(sizeof(struct sockaddr_in));
  *addr = addr_aux;
  socklen_t addrlen;
  return recvfrom(udp_descriptor,buffer,length,0,(struct sockaddr*)addr_aux,&addrlen);
}


int udpWriteToWithSockAddr(int udp_descriptor,char * mensage, int length,struct sockaddr_in server){

    size_t slen = sizeof(server);

    return sendto(udp_descriptor,mensage, length, 0, (struct sockaddr*) &server, slen);

}

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
