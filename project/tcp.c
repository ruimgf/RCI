#include <stdint.h>
#include "tcp.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int tcpBindListen(int server_port){
  int fd;
  struct sockaddr_in addr;
  if((fd=socket(AF_INET,SOCK_STREAM,0))==-1)
      return -1;


  memset((void*)&addr,(int)'\0',sizeof(addr));
  addr.sin_family=AF_INET;
  addr.sin_addr.s_addr=htonl(INADDR_ANY);
  addr.sin_port=htons(server_port);

  if(bind(fd,(struct sockaddr*)&addr,sizeof(addr))==-1){
      return -1;
  }


  if(listen(fd,5)==-1)
    return -1;

  return fd;
}

int tcpAccept(int myFd){
    int newFd;
    socklen_t addrlen;
    struct sockaddr_in addr;
    addrlen=sizeof(addr);
    if((newFd=accept(myFd,(struct sockaddr*)&addr,&addrlen))==-1)
      return -1;
    return newFd;
}

/**
 * [tcp_connect Realiza uma nova conexão ao servidor]
 * @param  tcp_server_ip   [Endereço do Servidor]
 * @param  tcp_server_port [Porta do Servidor]
 * @return                [Return]
 */
int tcpConnect(char * server_ip, int server_port){


  struct sockaddr_in server_addr;
  socklen_t len_endereco;
  int err;


  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (sock_fd == -1){
    return(-1);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(server_port);

  if (inet_aton(server_ip, &server_addr.sin_addr) == 0){
      printf("invalid address\n");
  }
  len_endereco = sizeof(server_addr);
  err = connect(sock_fd, (const struct sockaddr *) &server_addr,len_endereco);

  if (err == -1){
    return(-1);
  }

  return(sock_fd);
}


/**
 * [tcp_write Trata operções de escrita por parte dos clientes]
 * @param  tcp_descriptor [identificador do servidor]
 * @param  mensage           [chave]
 * @return               [-1 em caso de erro 0 no caso de sucesso]
 */
int tcpWrite(int tcp_descriptor,char * mensage, int length){

    //verificar argumentos
    int nsend = 0;
    int n;
    if (tcp_descriptor == -1 || mensage == NULL || length <= 0){
      return(-2);
    }
    while(nsend<length){
      n = send(tcp_descriptor,mensage+nsend,length-nsend, 0);
      if(n==-1)
          return -1;
      nsend += n;
    }


    return 0;

}


int tcpRead(int tcp_descriptor, char * buffer, int length){
  return recv(tcp_descriptor,buffer,length, 0);
}
