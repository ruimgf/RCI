#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "udp.h"
#include "tcp.h"
#include "messagelist.h"
#include "fdlist.h"
#include <time.h>
#include <signal.h>
#include "geral.h"

#define REFRESH_RATE 5

// Global Variables
messageList * m;
char test_reg[100];
appSpec appspec;
int reg = 0;
int fdIdUDP;
int fdIdTCPAccept;
msgserv msgservers[100];
fdList * msgservFd;
int num_msgservs = 0;


void wrongUse(){
  printf("Wrong Program Usage : msgserv –n name –j ip -u upt –t tpt [-i siip] [-p sipt] [–m m] [–r r] \n");
  exit(-1);
}

void close_correct(){
	int i;
	freeMessageList(m);
	freeFdList(msgservFd);
	for(i = 0; i<FdListLen(msgservFd); i++ ){
			int fd = getNFd(msgservFd,i);
			close(fd);
	}
	close(fdIdUDP);
	close(fdIdTCPAccept);
}

void readArgs(char ** argv,int argc){
	char aux[10];
  appspec.name[0]='\0';
  appspec.ip[0]='\0';
  appspec.upt=-1;
  appspec.tpt=-1;
	siPortIp(appspec.siip,&appspec.sipt);
  appspec.m=200;
  appspec.r=10;

  if(argc < 9 || argc > 17)
  {
		wrongUse();
		exit(-1);
  }
  else
  {
		int i;
    for(i=0; i<(argc-1)/2 ; i++)
    {
      if(strcmp(argv[1+2*i],"-n")==0)     //impares -n
      {
        strcpy(appspec.name, argv[2*i+2]);
      }
      if(strcmp(argv[1+2*i],"-j")==0)
      {
        strcpy(appspec.ip, argv[2*i+2]);
      }
      if(strcmp(argv[1+2*i],"-u")==0)
      {
        strcpy(aux, argv[2*i+2]);
        appspec.upt=atoi(aux);
      }
      if(strcmp(argv[1+2*i],"-t")==0)
      {
        strcpy(aux, argv[2*i+2]);
        appspec.tpt=atoi(aux);
      }
      if(strcmp(argv[1+2*i],"-i")==0)
      {
        strcpy(appspec.siip, argv[2*i+2]);
      }
      if(strcmp(argv[1+2*i],"-p")==0)
      {
        strcpy(aux, argv[2*i+2]);
        appspec.sipt=atoi(aux);
      }
      if(strcmp(argv[1+2*i],"-m")==0)
      {
        strcpy(aux, argv[2*i+2]);
        appspec.m=atoi(aux);
      }
      if(strcmp(argv[1+2*i],"-r")==0)
      {
        strcpy(aux, argv[2*i+2]);
        appspec.r=atoi(aux);
      }
    }
  }
  if(strlen(appspec.name)==0 || strlen(appspec.ip)==0 || appspec.upt==-1 || appspec.tpt==-1)
      wrongUse();
}

int max(int x, int y){
    if(x>y)
      return x;
    return y;
}

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

    insertMessageListEnd(m,message,-1);
		sprintf(buffer,"SMESSAGES\n%d;%s\n\n",m->actualLc,message);
		int i;
		for(i = 0; i<FdListLen(msgservFd); i++ ){
				int fdTCPread = getNFd(msgservFd,i);
				if(tcpWrite(fdTCPread,buffer,strlen(buffer))==-1){
					removeFdListEnd(msgservFd,fdTCPread);
					printf("Error one server go down\n");
				}
		}

    // send message to all servers

  }else if(strcmp(command,"GET_MESSAGES")==0){
    int n_messages;
    char * buffer_msg;

    sscanf(buffer,"%s %d",command,&n_messages);

    buffer_msg = getLastNmessages(m,n_messages);

    if(udpWriteToWithSockAddr(fdIdServer,buffer_msg,strlen(buffer_msg),*addr_client)==-1){
			exit(-1);
		}

    free(buffer_msg);

  }
  free(addr_client);

}

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
      getServers(fdIdServer,msgservers,&num_msgservs,appspec.siip,appspec.sipt);
			printServers(msgservers,num_msgservs);
    }else if(strcmp("show_messages",command)==0){
      printMessageList(m);
    }else if(strcmp("join",command)==0){
			reg = 1	;
			sprintf(test_reg,"REG %s;%s;%d;%d",appspec.name,appspec.ip,appspec.upt,appspec.tpt);
			if(udpWriteTo(fdIdServer,test_reg,strlen(test_reg),appspec.siip,appspec.sipt)==-1){
				printf("erro no registo\n");
				exit(-1);
			}
      printf("Registo com sucesso\n");
    }else if(strcmp("exit",command)==0){
      close_correct();
      exit(0);
    }else{
      printf("Unkown command\n");
    }
  }
}

void tcpRequest(int fdTCPread){
		char buffer[BUFFERSIZE];
		char command[50];
		int n;

		n = tcpRead(fdTCPread,buffer,BUFFERSIZE);
		if(n > 0){
			buffer[n] = '\0';
			sscanf(buffer,"%s\n",command);
			if(!strcmp(command,"SGET_MESSAGES")){
				char * send  = getAllMessages(m);
				tcpWrite(fdTCPread,send,strlen(send));
			}
			if(!strcmp(command,"SMESSAGES")){
				saveMessages(m,buffer);
			}
		}else{
			printf("One server go down\n");
			close(fdTCPread);
			removeFdListEnd(msgservFd,fdTCPread);
		}

}

void handler(int a){
		printf("um servidor foi a baixo\n");
}

int main(int argc, char *argv[])
{

  fd_set rfds;
  int fdMax;
	char buffer[BUFFERSIZE];
	int i;
	int fdSave;
	int fdTCPread;
	int counter=1	;
	void (*old_handler)(int);
	if((old_handler=signal(SIGPIPE,SIG_IGN))==SIG_ERR)
		exit(1);



  time_t select_ini, select_end;
	readArgs(argv,argc);
  m = createMessageList(appspec.m);
  fdIdUDP = udpServer(appspec.upt);

	fdIdTCPAccept = tcpBindListen(appspec.tpt);

  // connect to all and save fd

  getServers(fdIdUDP,msgservers,&num_msgservs,appspec.siip,appspec.sipt);

	msgservFd = createFdList();

  for (i = 0; i < num_msgservs; i++){
      if(strcmp(appspec.name,msgservers[i].name)==0){
        continue;
      }
      fdSave = tcpConnect(msgservers[i].ip,msgservers[i].tpt);
      if(fdSave!=-1){
					printf("connect\n");
          insertFdListEnd(msgservFd,fdSave);
          printf("%s %d\n",msgservers[i].ip, msgservers[i].tpt);
      }

  }

  // send message to get all messages
	struct timeval tr;
	int lenFdList = FdListLen(msgservFd);

	if(lenFdList > 0){
		// pedir mensagens todas

		sprintf(buffer,"SGET_MESSAGES\n");
		int p=0;
		int i;
		for(i=0;i<lenFdList;i++){
			int fdGetMessages = getNFd(msgservFd,p);

			if(tcpWrite(fdGetMessages,buffer,strlen(buffer))==-1){
					close(fdGetMessages);
			}
			FD_ZERO(&rfds);
			FD_SET(fdGetMessages,&rfds);
			tr.tv_usec = 0;
			tr.tv_sec = 1;
			counter=select(fdGetMessages+1,&rfds,(fd_set*)NULL,(fd_set*)NULL,&tr);

			if(counter > 0 ){// error try another
					int n = tcpRead(fdGetMessages,buffer,BUFFERSIZE);
					buffer[n] = '\0';
					saveMessages(m,buffer);
					break;
			}

		}
	}

  select_ini = time(0);

  tr.tv_usec = 0;
  select_end = select_ini;
  tr.tv_sec = REFRESH_RATE;

  while(1){
    FD_ZERO(&rfds);

    FD_SET(1,&rfds);
    fdMax = 0;
    FD_SET(fdIdUDP,&rfds);
    fdMax = max(fdMax, fdIdUDP);
    FD_SET(fdIdTCPAccept,&rfds);
		fdMax = max(fdIdTCPAccept, fdMax);

		for(i = 0; i<FdListLen(msgservFd); i++ ){
				fdTCPread = getNFd(msgservFd,i);
				FD_SET(fdTCPread,&rfds);
				fdMax = max(fdMax,fdTCPread);

		}


    counter=select(fdMax+1,&rfds,(fd_set*)NULL,(fd_set*)NULL,&tr);
    select_end = time(0);
    if(select_end - select_ini >= appspec.r){
      select_ini = time(0);
      sprintf(test_reg,"REG %s;%s;%d;%d",appspec.name,appspec.ip,appspec.upt,appspec.tpt);
			if(reg==1){
      	int n = udpWriteTo(fdIdUDP,test_reg,strlen(test_reg),appspec.siip,appspec.sipt);
      	if(n<=0){
        	printf("error reg\n");
        	exit(-1);
      	}
			}
      tr.tv_sec = appspec.r;
    }else{
      tr.tv_sec = appspec.r - (select_end - select_ini);
    }
    if(counter<0){
			printf("error on select\n");
      exit(1);//errror
    }

    if(counter > 0 ){
      if(FD_ISSET(fdIdUDP,&rfds)){
        readRmb(fdIdUDP);
      }
      if(FD_ISSET(1,&rfds)){
        keyboardRead(fdIdUDP);
      }
      if(FD_ISSET(fdIdTCPAccept,&rfds)){

        int fdSave = tcpAccept(fdIdTCPAccept);
        insertFdListEnd(msgservFd,fdSave);

      }

			for(i = 0; i<FdListLen(msgservFd); i++ ){
					fdTCPread = getNFd(msgservFd,i);
					if(FD_ISSET(fdTCPread,&rfds)){
						tcpRequest(fdTCPread);
		      }
			}

			// if para as leituras
    }


  }

	return 0;
}
