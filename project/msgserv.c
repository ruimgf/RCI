#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "udp.h"
#include "tcp.h"
#include "messagelist.h"
#include "fdlist.h"
#include <time.h>

#define FIELD_SEP ';'
#define LINE_SEP '\n'
#define BUFFERSIZE 2000
#define REFRESH_RATE 5

typedef struct msgserv_{
	char ip[16];    //endereço IP
	int upt;        //port udp
	int tpt;        //port tcp
}msgserv;

msgserv msgservers[100];
fdList * msgservFd;
int num_msgservs = 0;
// Global Variables
messageList * m;
char test_reg[100];
int lc = 0;
char name[100];
char ip[100];
char upt[100];
char tpt[100];
char siip[]  = "127.0.0.1";
int sipt = 59000;

void getServers(int myFd)
{
	int len=0;
	char port_udp[10],port_tcp[10];
	char * str, * str2;
	int nread;
  num_msgservs = 0;
  char buffer[BUFFERSIZE];
	if ((len=udpWriteTo(myFd, "GET_SERVERS", 11, siip, sipt)) >= 0)
	{
		printf ("UDP WRITE BYTES: %d\n", len);
		nread=udpRead(myFd, buffer, BUFFERSIZE);

		if (nread >= 0)
		{
			//printf("go clean");
			//write(1,buffer,nread);
			buffer[nread] = '\0';
			//printf("NREAD: %d, BUFFER: %s", nread, buffer);

			//descarta o SERVERS
			str=strchr(buffer,LINE_SEP);


			while((str=strchr(str,LINE_SEP)))
			{
				// Descarta o nome
				str = strchr(str,FIELD_SEP);
				if(!str) // Fim de tudo
					break;
				str++;

				// IP
				str2 = strchr(str,FIELD_SEP);
				if(!str2) // Mensagem mal definida
					break;

				len = strlen(str)-strlen(str2);
				strncpy(msgservers[num_msgservs].ip,str,len);
				msgservers[num_msgservs].ip[len] = '\0';

				// UDP
				str=++str2;
				str2=strchr(str,FIELD_SEP);
				if(!str2) // Mensagem mal definida
					break;
				len = strlen(str)-strlen(str2);
				strncpy(port_udp,str,len);
				port_udp[len]='\0';


				// TCP
				str=++str2;
				str2=strchr(str,LINE_SEP);
				if(!str2) // Mensagem mal definida
					break;

				len = strlen(str)-strlen(str2);
				strncpy(port_tcp,str,len);
				port_tcp[len]='\0';

				msgservers[num_msgservs].upt = atoi(port_udp);
				msgservers[num_msgservs].tpt = atoi(port_tcp);

				// Copia mensagem só no fim de todos os campos lidos
				// To DO ...

				printf("Servidor %d - IP: %s,\t UDP: %d,\t TCP: %d\n",num_msgservs,msgservers[num_msgservs].ip,msgservers[num_msgservs].upt,msgservers[num_msgservs].tpt);
				num_msgservs++;
			}
		}
	}
}



int max(int x, int y){
    if(x>y)
      return x;
    return y;
}

void wrongUse(){
  printf("Wrong Program Usage : msgserv –n name –j ip -u upt –t tpt [-i siip] [-p sipt] [–m m] [–r r] \n");
  exit(-1);
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

    insertMessageListEnd(m,message,lc);
		sprintf(buffer,"SMESSAGES\n%d;%s\n\n",lc,message);
		int i;
		for(i = 0; i<FdListLen(msgservFd); i++ ){
				int fdTCPread = getNFd(msgservFd,i);
				tcpWrite(fdTCPread,buffer,strlen(buffer));
		}
    lc++;
    // send message to all servers

  }else if(strcmp(command,"GET_MESSAGES")==0){
    int n_messages;
    char * buffer_msg;

    sscanf(buffer,"%s %d",command,&n_messages);

    buffer_msg = getLastNmessages(m,n_messages);

    udpWriteToWithSockAddr(fdIdServer,buffer_msg,strlen(buffer_msg),*addr_client);

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
      getServers(fdIdServer);
    }else if(strcmp("show_messages",command)==0){
      printMessageList(m);
    }else if(strcmp("join",command)==0){
      sprintf(test_reg,"REG %s;%s;%s;%s",name,ip,upt,tpt);
      udpWriteTo(fdIdServer,test_reg,strlen(test_reg),siip,sipt);
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

int tcpRequest(int fdTCPread){
		char buffer[BUFFERSIZE];
		char command[50];
		int n;
		n = tcpRead(fdTCPread,buffer,BUFFERSIZE);
		buffer[n] = '\0';
		sscanf(buffer,"%s\n",command);
		if(!strcmp(command,"SGET_MESSAGES")){
			char * send  = getAllMessages(m);
			printf("%d\n",fdTCPread);
			tcpWrite(fdTCPread,send,strlen(send));
		}
		if(!strcmp(command,"SMESSAGES")){
			saveMessages(m,buffer);
		}
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

  int fdIdUDP = udpServer(atoi(upt));
  int fdIdTCPAccept = tcpBindListen(atoi(tpt));

  // connect to all and save fd

	printf("%s\n","ola");
  getServers(fdIdUDP);
  msgservFd = createFdList();
  printf("go connect\n");
  for (i = 0; i < num_msgservs; i++){
      if(msgservers[i].tpt == atoi(tpt))// comparar ip tb
        continue;
      fdSave = tcpConnect(msgservers[i].ip,msgservers[i].tpt);
      if(fdSave!=-1){
          insertFdListEnd(msgservFd,fdSave);
          printf("%s %d\n",msgservers[i].ip, msgservers[i].tpt);
      }

  }
  printf("end connect\n");
  // send message to get all messages
	struct timeval tr;
	int lenFdList = FdListLen(msgservFd);
	printf("end len\n");
	if(lenFdList > 0){
		// pedir mensagens todas

		sprintf(buffer,"SGET_MESSAGES\n");
		int p=0;
		while(1){
			int fdGetMessages = getNFd(msgservFd,p);
			printf("go send\n");
			tcpWrite(fdGetMessages,buffer,strlen(buffer));
			FD_ZERO(&rfds);
			FD_SET(fdGetMessages,&rfds);
			tr.tv_usec = 0;
			tr.tv_sec = 1;
			//counter=select(fdGetMessages+1,&rfds,(fd_set*)NULL,(fd_set*)NULL,&tr);
			printf("end select\n");
			if(counter == 0 ){// error try another
				p++;
			}else{
					int n = tcpRead(fdGetMessages,buffer,BUFFERSIZE);
					buffer[n] = '\0';
					printf("%s\n",buffer);
					saveMessages(m,buffer);
					printf("save\n",buffer);
					//save msg
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
    if(select_end - select_ini >= REFRESH_RATE){
      select_ini = time(0);
      sprintf(test_reg,"REG %s;%s;%s;%s",name,ip,upt,tpt);
      int n = udpWriteTo(fdIdUDP,test_reg,strlen(test_reg),siip,sipt);
      if(n<=0){
        printf("error reg\n");
        exit(-1);
      }
      tr.tv_sec = REFRESH_RATE;
    }else{
      tr.tv_sec = REFRESH_RATE - (select_end - select_ini);
    }
    if(counter<0){
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
        printf("go accept\n");
        int fdSave = tcpAccept(fdIdTCPAccept);
        insertFdListEnd(msgservFd,fdSave);
        printf("accept sucess\n");
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
  close(fdIdUDP);

}
