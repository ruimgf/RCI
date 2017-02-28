#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "udp.h"


#define BUFFERSIZE 100

void wrong_use(){
  printf("Wrong Program Usage : msgserv –n name –j ip -u upt –t tpt [-i siip] [-p sipt] [–m m] [–r r] \n");
  exit(-1);
}

int main(int argc, char *argv[]){
  char buffer[BUFFERSIZE];
  char command[BUFFERSIZE];
  char name[] = "rui";
  char ip[] = "127.0.0.1";
  char upt[] = "9000";
  char tpt[] = "10000";
  char ip_tejo[]  = "193.136.138.142";
  int dns_port = 59000;
  char test_reg[] = "REG test1234;127.0.0.1;1000;2000\n";

  int fd_id_server = udp_connect(ip_tejo,dns_port);
  printf("go WRITE\n");
  udp_write_to(fd_id_server,test_reg,strlen(test_reg),ip_tejo,dns_port);
  /*
  if(argc < 9){
    wrong_use();
  }
  */


 /*
      Adicionar parte para validar argv
  */

  // 1 -  Registar servidor de mensagens no sevidor de identidades
  // 2 - interrogar o servido de mensagens sobre a identidade dos outros servidores de mensagens
  //  e estabelecer TCP entre cada um destes
  // Ir buscar todas as mensagens
  //
  while(1){
    printf("Enter a message: \n");
    if(fgets(buffer, BUFFERSIZE , stdin) != NULL)
    {
      size_t ln = strlen(buffer)-1;
      if (buffer[ln] == '\n'){
          buffer[ln] = '\0';
      }
      sscanf(buffer,"%s",command);

      if(strcmp("show_servers",command)==0){
          printf("Show Servers\n");

      }else if(strcmp("show_messages",command)==0){
          printf("Show mensagens\n");

      }else if(strcmp("join",command)==0){
        printf("Go Registar\n");

      }else if(strcmp("exit",command)==0){
        printf("exit");
      }else{
        printf("Unkown command\n");
      }
    }

  }
}
