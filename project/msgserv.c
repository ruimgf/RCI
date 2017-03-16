#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "udp.h"


#define BUFFERSIZE 100
#define MESSAGESIZE 140



void wrongUse(){
  printf("Wrong Program Usage : msgserv –n name –j ip -u upt –t tpt [-i siip] [-p sipt] [–m m] [–r r] \n");
  //exit(-1);
}

typedef struct messageBox_{
    int actual_pos; // actual first free position
    char ** messages; // matrix of messages
    int length; // max length off box
}messageBox;

messageBox * m;

messageBox * createMessageBox(int length){
      messageBox * m;
      int i;
      m = malloc(sizeof(messageBox));
      m->actual_pos = 0;
      m->length = length;
      m->messages = malloc(length * sizeof(char *));
      for(i=0;i<MESSAGESIZE;i++){
        m->messages[i] = malloc(MESSAGESIZE * sizeof(char));
      }
      return m;
}

void saveMessage(messageBox * m ,char * message){
     sprintf(m->messages[m->actual_pos],"%s",message);
     m->actual_pos = m->actual_pos + 1;
}

void printAllMessages(messageBox * m){
    int i;
    // cuidado se ja deu a volta ou nao
    for (i = 0; i < m->actual_pos; i++) {
      printf("%s",m->messages[i]);
    }
}

void readRmb(int fdIdServer){
  char buffer[200];
  char command[50];
  int n = udpRead(fdIdServer,buffer,BUFFERSIZE);
  sscanf(buffer,"%s",command);
  if(strcmp(command,"PUBLISH")==0){
    char message[140];
    strncpy(message, buffer+8, 140);
    saveMessage(m,message);
  }
}

void destructMessageBox(messageBox * m){
    int i;
    for ( i = 0; i < m->length; i++) {
      free(m->messages[i]);
    }
    free(m->messages);
    free(m);
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

// isto ainda é para mudar
char name[100];
char ip[100];
char upt[100];
char tpt[100];
char ip_tejo[]  = "193.136.138.142";
int dns_port = 59000;
char test_reg[100];

void keyboardRead(int fdIdServer){
  char buffer[BUFFERSIZE];
  char command[BUFFERSIZE];

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
      printAllMessages(m);
    }else if(strcmp("join",command)==0){
      sprintf(test_reg,"REG %s;%s;%s;%s",name,ip,upt,tpt);
      udpWriteTo(fdIdServer,test_reg,strlen(test_reg),ip_tejo,dns_port);
      // pode implementar-se um read para ver se foi registado com sucesso
      printf("Go Registar\n");
    }else if(strcmp("exit",command)==0){
      destructMessageBox(m);
      close(fdIdServer);
      exit(0);
    }else{
      printf("Unkown command\n");
    }
  }
}
int main(int argc, char *argv[])
{
  char buffer[BUFFERSIZE];
  char command[BUFFERSIZE];

  fd_set rfds;

  m = createMessageBox(200);

  // trocar a ordem disto, pode aparecer por outras ordens
  if(argc < 9){
    wrongUse();
  }else{
    if(strcmp(argv[1],"-n")==0){
      strcpy(name,argv[2]);
    }else{
      wrongUse();
    }
    if(strcmp(argv[3],"-j")==0){
      strcpy(ip,argv[4]);
    }else{
      wrongUse();
    }
    if(strcmp(argv[5],"-u")==0){
      strcpy(upt,argv[6]);
    }else{
      wrongUse();
    }
    if(strcmp(argv[7],"-t")==0){
      strcpy(tpt,argv[8]);
    }else{
      wrongUse();
    }
    // falta adicionar os opcionais
  }

  int fdIdServer = udpServer(5000);

  while(1){
    FD_ZERO(&rfds);
    FD_SET(fdIdServer,&rfds);
    FD_SET(1,&rfds);
    int maxfd=fdIdServer;
    int counter;

    counter=select(fdIdServer+1,&rfds,(fd_set*)NULL,(fd_set*)NULL,(struct timeval *)NULL);

    if(counter<=0)exit(1);//errror

    if(FD_ISSET(fdIdServer,&rfds)){
      readRmb(fdIdServer);
    }else if(FD_ISSET(1,&rfds)){
      keyboardRead(fdIdServer);
    }
  }

}
