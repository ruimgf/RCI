#include "udp.h"

int udpSockect()
{
  struct sockaddr_in server_addr;
  socklen_t len_endereco;
  int err;

  int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

  if (sock_fd == -1)
    return(-1);
    /*
  if(server_ip == NULL || server_port <= 0){
    return(-2);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(server_port);

  if (inet_aton(server_ip, &server_addr.sin_addr) == 0){

      printf("invalid address\n");
  }

  err = connect(sock_fd, (const struct sockaddr *) &server_addr,sizeof(server_addr));

  if (err == -1){
    printf("conn error\n");
    return(-1);
  }
  */
  return(sock_fd);
}

void udpClose(int udp_descriptor)
{
  if (udp_descriptor == -1 )
    return;
  close(udp_descriptor);
}

int udpWriteTo(int udp_descriptor,char * mensage, int length,char * ip, int port)
{
    char buffer[100];
    //verificar argumentos
    struct sockaddr_in server;
    size_t slen = sizeof(server);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    inet_aton(ip, &server.sin_addr);
    if (udp_descriptor == -1 || mensage == NULL || length <= 0  || ip == NULL || port < 0)
      return(-2);
    
    return sendto(udp_descriptor,mensage, length, 0, (struct sockaddr*) &server, slen);
}

int udpRead(int udp_descriptor, char * buffer, int length)
{
  struct sockaddr_in addr;
  int nread=0, addrlen;

  addrlen = sizeof(addr);	
  //verificar argumentos
  if (udp_descriptor == -1 || buffer == NULL || length <= 0)
    return(-2);

    /** pode haver aquui um erro se m_s.value_length > sizeof(value) **/
  return recvfrom(udp_descriptor,buffer,length,0,(struct sockaddr*)&addr,&addrlen);
}


int udpReadAndGetSender(int udp_descriptor, char * buffer, int length , struct sockaddr_in ** addr){
  struct sockaddr_in * addr_aux = malloc(sizeof(struct sockaddr_in));
  *addr = addr_aux;
  int nread=0, addrlen;
  //verificar argumentos
  if (udp_descriptor == -1 || buffer == NULL || length <= 0){
    return(-2);
  }

  nread=recvfrom(udp_descriptor,buffer,length,0,(struct sockaddr*)addr_aux,&addrlen);
  return nread;
}


int udpWriteToWithSockAddr(int udp_descriptor,char * mensage, int length,struct sockaddr_in server){
    //verificar argumentos
    size_t slen = sizeof(server);
    if (udp_descriptor == -1 || mensage == NULL || length <= 0){
      return(-2);
    }

    sendto(udp_descriptor,mensage, length, 0, (struct sockaddr*) &server, slen);

    return 0;

}

int udpServer(int port){
  int fd = socket(AF_INET,SOCK_DGRAM,0);
  struct sockaddr_in serveraddr;

  memset((void*)&serveraddr,(int)'\0',sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((u_short)port);

  bind(fd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
  return fd;
}
