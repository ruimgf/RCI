#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "udp.h"

#define BUFFERSIZE 170

// global variables
int myFd;
char msgservIp[] = "127.0.0.1";
int msgservPort = 2115;

int publishMessage(char * message){
    udpWriteTo(myFd,message,strlen(message),msgservIp, msgservPort);
    // add here code to send message to server
    return 0;
}

void keyboardRead()
{
  char buffer[BUFFERSIZE];
  char command[30];
  char message[140];

  if(fgets(buffer, BUFFERSIZE , stdin) != NULL)
  {
    // Retirar \n da linha lida
    size_t ln = strlen(buffer)-1;
    if (buffer[ln] == '\n'){
        buffer[ln] = '\0';
    }
    sscanf(buffer,"%s",command);

    if(strcmp("show_servers",command)==0){
        printf("Show Servers\n");
    }else if(strcmp("publish",command)==0){
        strncpy(message, buffer+8, 140);
        printf("publish mensage\n");
        publishMessage(message);
    }else if(strcmp("show_last_mensages",command)==0){
        printf("Show mensagens\n");
    }else if(strcmp("exit",command)==0){
        exit(0);
    }else{
      printf("Unkown command\n");
    }
  }
  printf("Enter a command:  ");
}

int main(int argc, char ** argv){

  myFd = udpConnect();
  // mandar mensagem para ir buscar os servidores
  printf("Enter a command:  ");
  while(1){
    keyboardRead();
  }

  return 1;
}
