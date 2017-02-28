#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "udp.h"


#define BUFFERSIZE 100

void wrong_use(){
  printf("Wrong Program Usage : msgserv –n name –j ip -u upt –t tpt [-i siip] [-p sipt] [–m m] [–r r] \n");
  //exit(-1);
}

int main(int argc, char *argv[]){
  char buffer[BUFFERSIZE];
  char command[BUFFERSIZE];
  char name[100];
  char ip[100];
  char upt[100];
  char tpt[100];
  char ip_tejo[]  = "193.136.138.142";
  int dns_port = 59000;
  char test_reg[100];


  // validar argumentos de chamada
  if(argc < 9){
    wrong_use();
  }else{
    if(strcmp(argv[1],"-n")==0){
      strcpy(name,argv[2]);
    }else{
      wrong_use();
    }
    if(strcmp(argv[3],"-j")==0){
      strcpy(ip,argv[4]);
    }else{
      wrong_use();
    }
    if(strcmp(argv[5],"-u")==0){
      strcpy(upt,argv[6]);
    }else{
      wrong_use();
    }
    if(strcmp(argv[7],"-t")==0){
      strcpy(tpt,argv[8]);
    }else{
      wrong_use();
    }
    // falta adicionar os opcionais
  }

  // 1 -  Registar servidor de mensagens no sevidor de identidades
  // 2 - interrogar o servido de mensagens sobre a identidade dos outros servidores de mensagens
  //  e estabelecer TCP entre cada um destes
  // Ir buscar todas as mensagens
  //
  //

  // mudar isto para ir buscar o ip
  int fd_id_server = udp_connect(ip_tejo,dns_port);
  while(1){
    printf("Enter a command:  ");
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
        sprintf(test_reg,"REG %s;%s;%s;%s",name,ip,upt,tpt);
        udp_write_to(fd_id_server,test_reg,strlen(test_reg),ip_tejo,dns_port);
        printf("Go Registar\n");

      }else if(strcmp("exit",command)==0){
        exit(0);
      }else{
        printf("Unkown command\n");
      }
    }

  }
}
