#include "udp.h"

/**
 * [udp_connect Realiza uma nova conexão ao servidor]
 * @param  udp_server_ip   [Endereço do Servidor]
 * @param  udp_server_port [Porta do Servidor]
 * @return                [Return]
 */
int udpConnect(){

  struct sockaddr_in server_addr;
  socklen_t len_endereco;
  int err;

  int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

  if (sock_fd == -1){
    return(-1);
  }
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

/**
 * [udp_close Termina a ligação entre cliente e servidor]
 * @param udp_descriptor [Indentificador do servidor]
 */
void udpClose(int udp_descriptor){

  if (udp_descriptor == -1 ){
    return;
  }
  close(udp_descriptor);

}
/**
 * [udp_write Trata operções de escrita por parte dos clientes]
 * @param  udp_descriptor [identificador do servidor]
 * @param  mensage           [chave]
 * @return               [-1 em caso de erro 0 no caso de sucesso]
 */
int udpWriteTo(int udp_descriptor,char * mensage, int length,char * ip, int port){
    char buffer[100];
    //verificar argumentos
    struct sockaddr_in server;
    size_t slen = sizeof(server);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    inet_aton(ip, &server.sin_addr);
    if (udp_descriptor == -1 || mensage == NULL || length <= 0  || ip == NULL || port < 0){
      return(-2);
    }

    sendto(udp_descriptor,mensage, length, 0, (struct sockaddr*) &server, slen);

    return 0;

}

/**
 * [udp_read operação leitura por parte do cliente]
 * @param  udp_descriptor [identificador do servidor]
 * @param  buffers           [chave]
 * @param  length  [tamanho para receber valor]
 * @return               [nr_bytes lidos ou -1,-2 em casos de erro]
 */
int udpRead(int udp_descriptor, char * buffer, int length){
  int nread=0;
  //verificar argumentos
  if (udp_descriptor == -1 || buffer == NULL || length <= 0){
    return(-2);
  }
    /** pode haver aquui um erro se m_s.value_length > sizeof(value) **/
  nread=recv(udp_descriptor,buffer,length, 0);
  if(nread==-1){
    return(-1);
  }
  return nread;
}
