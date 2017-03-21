#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "udp.h"
#include "messagelist.h"
#include <time.h>


#define BUFFERSIZE 2000
#define REFRESH_RATE 5

void wrongUse(){
  printf("Wrong Program Usage : msgserv –n name –j ip -u upt –t tpt [-i siip] [-p sipt] [–m m] [–r r] \n");
  exit(-1);
}

messageList * m;
char test_reg[100];
int lc = 0;

void readRmb(int fdIdServer){
  char buffer[BUFFERSIZE];
  char command[50];
  struct sockaddr_in * addr_client;
  int n = udpReadAndGetSender(fdIdServer,buffer,BUFFERSIZE,&addr_client);
  // if rmb send \0 or not i will not have problems with str functions
  if(n == -1){
    printf("erro n\n");
    exit(-2);
  }
  buffer[n] = '\0';
  size_t ln = strlen(buffer)-1;
  if (buffer[ln] == '\n'){
      buffer[ln] = '\0';
  }
  sscanf(buffer,"%s",command);

  if(strcmp(command,"PUBLISH")==0){
    char message[141];

    strncpy(message, buffer+8, 140);
    message[140] = '\0';

    insertMessageListEnd(m,message,lc);
    lc++;

  }else if(strcmp(command,"GET_MESSAGES")==0){
    int n_messages;
    char * buffer_msg;

    sscanf(buffer,"%s %d",command,&n_messages);

    buffer_msg = getLastNmessages(m,n_messages);

    udpWriteToWithSockAddr(fdIdServer,buffer_msg,strlen(buffer_msg),*addr_client);

    free(buffer_msg);
    //udpWriteTo(fdIdServer,buffer,strlen(buffer),char * ip, int port);
  }
  free(addr_client);

}



// isto ainda é para mudar

char name[100];
char ip[100];
char upt[100];
char tpt[100];
char ip_tejo[]  = "193.136.138.142";
int dns_port = 59000;


void keyboardRead(int fdIdServer){
  char buffer[BUFFERSIZE];
  char command[BUFFERSIZE];

  if(fgets(buffer, BUFFERSIZE , stdin) != NULL)
  {
    size_t ln = strlen(buffer)-1;
    if (buffer[ln] == '\n'){
        buffer[ln] = '\0';
    }
    if(ln == 0){
      printf("Unkown command\n");
      return;
    }
    sscanf(buffer,"%s",command);

    if(strcmp("show_servers",command)==0){
      printf("Show Servers\n");
    }else if(strcmp("show_messages",command)==0){
      printMessageList(m);
    }else if(strcmp("join",command)==0){
      sprintf(test_reg,"REG %s;%s;%s;%s",name,ip,upt,tpt);
      udpWriteTo(fdIdServer,test_reg,strlen(test_reg),ip_tejo,dns_port);
      // pode implementar-se um read para ver se foi registado com sucesso
      printf("Go Registar\n");
    }else if(strcmp("exit",command)==0){
      freeMessageList(m);
      close(fdIdServer);
      exit(0);
    }else{
      printf("Unkown command\n");
    }
  }
}

int main(int argc, char *argv[])
{

  fd_set rfds;
  m = createMessageList();
  time_t select_ini, select_end;
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

  select_ini = time(0);
  struct timeval tr;
  tr.tv_usec = 0;
  select_end = select_ini;
  tr.tv_sec = REFRESH_RATE;
  while(1){
    FD_ZERO(&rfds);
    FD_SET(fdIdServer,&rfds);
    FD_SET(1,&rfds);
    int maxfd=fdIdServer;
    int counter;


    counter=select(fdIdServer+1,&rfds,(fd_set*)NULL,(fd_set*)NULL,&tr);
    select_end = time(0);
    if(select_end - select_ini >= REFRESH_RATE){
      select_ini = time(0);
      sprintf(test_reg,"REG %s;%s;%s;%s",name,ip,upt,tpt);
      int n = udpWriteTo(fdIdServer,test_reg,strlen(test_reg),ip_tejo,dns_port);
      //printf("SEND REG %d\n",n);
      tr.tv_sec = REFRESH_RATE;
    }else{
      tr.tv_sec = REFRESH_RATE - (select_end - select_ini);
    }
    if(counter<0){
      exit(1);//errror
    }

    if(counter > 0 ){
      if(FD_ISSET(fdIdServer,&rfds)){
        readRmb(fdIdServer);
      }
      if(FD_ISSET(1,&rfds)){
        keyboardRead(fdIdServer);
      }
    }


  }
  close(fdIdServer);

}
