#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "udp.h"

#define BUFFERSIZE 170
int fd_id_server =
int main(int argc, char ** argv){
  char buffer[170];
  char command[30];
  char server_ip[] = "127.0.0.1";
  int server_port;
  int fd_udp = udp_connect();
  // mandar mensagem para ir buscar os servidores
  //
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
      }else if(strcmp("publish",command)==0){
          printf("Show mensagens\n");
      }else if(strcmp("show_last_menssages",command)==0){
          printf("Show mensagens\n");
      }else if(strcmp("exit",command)==0){
        exit(0);
      }else{
        printf("Unkown command\n");
      }
    }

  }


}
