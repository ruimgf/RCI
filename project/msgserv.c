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
void help()
{
	printf("\n MSGSERV APP COMMANDS\n");
	printf(" show_servers             -identities of registed message servers \n");
	printf(" join                     -regist in identities server \n");
	printf(" show_messages            -show messages saved in message servers \n");
	printf(" exit                     -application exit\n\n");
}


/**
 * [wrongUse description]
 */
void wrongUse(){
  printf("Wrong Program Usage : msgserv –n name –j ip -u upt –t tpt [-i siip] [-p sipt] [–m m] [–r r] \n");
  exit(-1);
}

/**
 * [close_correct function to exit]
 */
void close_correct(){
	int i;
	freeMessageList(m);
	for(i = 0; i<FdListLen(msgservFd); i++ ){
			int fd = getNFd(msgservFd,i);
			close(fd);
	}
  freeFdList(msgservFd);
	close(fdIdUDP);
	close(fdIdTCPAccept);
}

/**
 * [readArgs parse input args]
 * @param argv
 * @param argc
 */
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

/**
 * [max]
 * @param  x [description]
 * @param  y [description]
 * @return   max between x and y
 */
int max(int x, int y){
    if(x>y)
      return x;
    return y;
}

/**
 * [readRmb process requests from CLIENT]
 * @param fdIdServer [my udp SOCKET]
 */
void readRmb(int fdIdServer){
  char buffer[BUFFERSIZE];
  char command[50];
  struct sockaddr_in * addr_client;
  socklen_t slen;
  int n = udpReadAndGetSender(fdIdServer,buffer,BUFFERSIZE,&addr_client,&slen);
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
  if(sscanf(buffer,"%s",command)!=1){
    printf("ERROR : read command\n");
    return;
  }

  if(strcmp(command,"PUBLISH")==0){
    char message[141];

    strncpy(message, buffer+8, 140);
    message[140] = '\0';

    insertMessageListEnd(m,message,-1);
		sprintf(buffer,"SMESSAGES\n%d;%s\n\n",m->actualLc-1,message);
		int i;
		for(i = 0; i<FdListLen(msgservFd); i++ ){
				int fdTCPread = getNFd(msgservFd,i);
				if(tcpWrite(fdTCPread,buffer,strlen(buffer))==-1){
					removeFdListEnd(msgservFd,fdTCPread);
					printf("ERROR: one server went down\n");
				}


		}

    // send message to all servers

  }else if(strcmp(command,"GET_MESSAGES")==0){
    int n_messages;
    char * buffer_msg;

    if(sscanf(buffer,"%s %d",command,&n_messages)!=2){
			printf("invalid request\n");
			return;
		}

    buffer_msg = getLastNmessages(m,n_messages);

    if(udpWriteToWithSockAddr(fdIdServer,buffer_msg,strlen(buffer_msg),addr_client,&slen)==-1){
			exit(-1);
		}

    free(buffer_msg);

  }
  free(addr_client);

}
/**
 * [keyboardRead process request from keyboard]
 * @param fdIdServer [fd of udp socket]
 */
void keyboardRead(int fdIdServer){
  char buffer[BUFFERSIZE];
  char command[BUFFERSIZE];
  fd_set rfds;
  struct timeval tr;
  tr.tv_usec = 0;
  tr.tv_sec =  5;
  if(fgets(buffer, BUFFERSIZE , stdin) != NULL)
  {
    size_t ln = strlen(buffer)-1;
    if (buffer[ln] == '\n'){
        buffer[ln] = '\0';
    }
    if(ln == 0){
      printf("Unkown command\n");
      help();
      return;
    }
    if(sscanf(buffer,"%s",command)!=1){
      printf("ERROR : read command \n");
      return;
    }

    if(strcmp("show_servers",command)==0){
      printFdList(msgservFd);
    }else if(strcmp("show_messages",command)==0){
      printMessageList(m);
    }else if(strcmp("join",command)==0&&reg==0){

          reg = 1	;
          
          getServers(fdIdUDP,msgservers,&num_msgservs,appspec.siip,appspec.sipt);
			sprintf(test_reg,"REG %s;%s;%d;%d",appspec.name,appspec.ip,appspec.upt,appspec.tpt);
			if(udpWriteTo(fdIdServer,test_reg,strlen(test_reg),appspec.siip,appspec.sipt)==-1){
				printf("ERROR: in registration\n");
				exit(-1);
			}
    			
          // connect to all servers
          int i;
          for (i = 0; i < num_msgservs; i++){
              if(strcmp(appspec.name,msgservers[i].name)==0){ // if this server on list ingnore it
                continue;
              }
              printf("Trying to connect to %s on port %d\n",msgservers[i].ip,msgservers[i].tpt);
              int fdSave = tcpConnect(msgservers[i].ip,msgservers[i].tpt);
              if(fdSave!=-1){ // save fd
                  printf("CONNECTED\n");
                  insertFdListEnd(msgservFd,fdSave,msgservers[i].name,msgservers[i].ip,msgservers[i].tpt,msgservers[i].upt);
                  
              }else{
				  printf("ERROR\n");
			  }

          }

        int lenFdList = FdListLen(msgservFd);

        if(lenFdList > 0){
          // request messages

          sprintf(buffer,"SGET_MESSAGES\n");


          int i;
          for(i=0;i<lenFdList;i++){
            int fdGetMessages = getNFd(msgservFd,i);
						
            if(tcpWrite(fdGetMessages,buffer,strlen(buffer))==-1){
                printf("error write");
                close(fdGetMessages);
            }
            FD_ZERO(&rfds);
            FD_SET(fdGetMessages,&rfds);
            tr.tv_usec = 0;
            tr.tv_sec = 5;

            int counter=select(fdGetMessages+1,&rfds,(fd_set*)NULL,(fd_set*)NULL,&tr);

            if(counter > 0 ){// error try another

                int n;
                int nread=0;
                while(1){

                  n = tcpRead(fdGetMessages,buffer+nread,BUFFERSIZE-nread); // quando há muitas mensagens só numa leitura não funciona
                  nread += n;
                  buffer[nread] = '\0';

                  if(saveMessages(m,buffer)==0){ // concluido com sucesso, mensagem completa

										break;
                  }
									FD_ZERO(&rfds);
			            FD_SET(fdGetMessages,&rfds);
									counter=select(fdGetMessages+1,&rfds,(fd_set*)NULL,(fd_set*)NULL,&tr);
									if(counter==0){
										printf("error read\n");
										exit(-1);
									}
                }
                break; // sucess
            }

          }
          printf("Success in registration\n");
        }
    }else if(strcmp("exit",command)==0){
      close_correct();
      exit(0);
    }else if(strcmp("join",command)==0&&reg==1){
		printf("join yet done\n");
    }else{
      printf("Unkown command\n");
      help();
    }
  }
}
/**
 * [tcpRequest process a request from another server]
 * @param fdTCPread [other server fd]
 */
void tcpRequest(int fdTCPread){
		char buffer[BUFFERSIZE];
		char command[50];
		int n;

		n = tcpRead(fdTCPread,buffer,BUFFERSIZE);

		if(n > 0){
			buffer[n] = '\0';
			if(sscanf(buffer,"%s\n",command)!=1){
				printf("invalid request\n");
				return;
			}
			if(!strcmp(command,"SGET_MESSAGES")){
				char * send  = getAllMessages(m);
				if(tcpWrite(fdTCPread,send,strlen(send))==-1){
					printf("ERROR write\n");
					return;
				}
			}
			if(!strcmp(command,"SMESSAGES")){
				saveMessages(m,buffer);
			}
		}else{
			close(fdTCPread);
			removeFdListEnd(msgservFd,fdTCPread);
		}

}


int main(int argc, char *argv[])
{

  fd_set rfds;
  int fdMax;
	int i;
	int fdTCPread;
	int counter=1	;
	char ipAccept[18];
	int tptAccept;
	void (*old_handler)(int);

  // ignore signal SIGPIPE
	if((old_handler=signal(SIGPIPE,SIG_IGN))==SIG_ERR)
		exit(1);

  time_t select_ini, select_end;

  readArgs(argv,argc);

  m = createMessageList(appspec.m);
  msgservFd = createFdList();
  fdIdUDP = udpServer(appspec.upt);
	if(fdIdUDP==-1){
		printf("error create UDP socket\n");
		exit(-1);
	}
	fdIdTCPAccept = tcpBindListen(appspec.tpt);
	if(fdIdTCPAccept==-1){
		printf("error create tcp socket\n");
		exit(-1);
	}


  struct timeval tr;
  select_ini = time(0);

  tr.tv_usec = 0;
  select_end = select_ini;
  tr.tv_sec = appspec.r;
  printf(">> ");
  fflush(stdout);
  while(1){

    // set all fd
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

    // wait for a request
    counter=select(fdMax+1,&rfds,(fd_set*)NULL,(fd_set*)NULL,&tr);
    select_end = time(0);

    if(select_end - select_ini >= appspec.r){ // if is time to refresh registration
      select_ini = time(0);
      sprintf(test_reg,"REG %s;%s;%d;%d",appspec.name,appspec.ip,appspec.upt,appspec.tpt);
			if(reg==1){
      	int n = udpWriteTo(fdIdUDP,test_reg,strlen(test_reg),appspec.siip,appspec.sipt);
      	if(n<=0){
        	printf("ERROR: in registration in identities server\n");
        	exit(-1);
      	}
			}
      tr.tv_sec = appspec.r;
    }else{
      tr.tv_sec = appspec.r - (select_end - select_ini);
    }
    if(counter<0){
			printf("ERROR: in select\n");
      exit(1);//errror
    }
    // check all fd
    if(counter > 0 ){
      if(FD_ISSET(fdIdUDP,&rfds)){
        readRmb(fdIdUDP);
      }
      if(FD_ISSET(1,&rfds)){ // keyboard
        keyboardRead(fdIdUDP);
        printf(">>  ");
        fflush(stdout);
      }

      if(FD_ISSET(fdIdTCPAccept,&rfds)){
        int fdSave = tcpAccept(fdIdTCPAccept,ipAccept,&tptAccept);
				if(fdSave != -1){
					insertFdListEnd(msgservFd,fdSave,"Unkown",ipAccept,-1,tptAccept);
				}



      }

			for(i = 0; i<FdListLen(msgservFd); i++ ){
					fdTCPread = getNFd(msgservFd,i);
					if(FD_ISSET(fdTCPread,&rfds)){
						tcpRequest(fdTCPread);
		      }
			}

    }


  }

	return 0;
}
