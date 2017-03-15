#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "udp.h"

#define BUFFERSIZE 1024

// global variables
int myFd;
int msgservPort = 2115;
char msgservIp[] = "127.0.0.1";


typedef struct msgserv_{
	char ip[16]; //endereço IP
	int upt;     //port udp
	int tpt;     //port tcp
}msgserv
	 

void help(){
	
	printf(" RMB APP COMMANDS\n");
	printf(" show_servers             -identities of registed message servers \n");
	printf(" publish message          -text message publication with 140 characters maximum \n");
	printf(" show_lastest_messages n  -last n messages saved in message servers \n");
	printf(" exit                     -application exit\n");
}


int publishMessage(char* message){
    udpWriteTo(myFd,message,strlen(message),msgservIp, msgservPort);
    // add here code to send message to server
    return 0;
}


void keyboardRead(char* siip, int siport)
{
	char buffer[BUFFERSIZE];
	char buffercpy[BUFFERSIZE];
	char command[30];
	char message[140];
	int nread=0;
	
	char * pos;

	if(fgets(buffer, BUFFERSIZE , stdin) != NULL)
	{
		// Retirar \n da linha lida
		size_t ln = strlen(buffer)-1;
		if (buffer[ln] == '\n')
			buffer[ln] = '\0';
		
		sscanf(buffer,"%s",command);

		if(strcmp("show_servers",command)==0)
		{
			printf("Show Servers\n");
			
			udpWriteTo(myFd, "GET_SERVERS", 11, siip, siport);
			nread=udpRead(myFd, buffer, BUFFERSIZE);
			write(1,buffer,nread);		
			pos=strchr(buffer,'\n');
			strcpy(buffercpy,&buffer[pos]);
			while(strchr(buffer,'\n')!=NULL)
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
		}
		else if(strcmp("publish",command)==0)
		{
			strncpy(message, buffer+8, 140);
			printf("publish mensage\n");
			publishMessage(message);
		}
		else if(strcmp("show_last_mensages",command)==0)
		{
			printf("Show mensagens\n");
		}
		else if(strcmp("exit",command)==0)
		{
			exit(0);
		}
		else if(strcmp("help",command)==0)
		{
			help();
		}
		else
		{
			printf("Unkown command\n");
			help();
		}
	}
	printf("Enter a command: ");
}


void siPortIp(char* siip, int* sipt)
{	
	struct hostent *serverid;
	struct in_addr *serverid_ip;
	if((serverid=gethostbyname("tejo.tecnico.ulisboa.pt"))==NULL)
		exit(1);
	serverid_ip=(struct in_addr*)serverid->h_addr_list[0];
	
	sprintf(siip,"%s",inet_ntoa(*serverid_ip));
	*sipt = 59000;	
}


int main(int argc, char ** argv){

	char siip[16];
	int sipt;
	
	if (argc>1)
	{
		if(strcmp(argv[1],"-i")==0 && strcmp(argv[3],"-p")==0)
		{
			strcpy(siip,argv[2]);
			sipt=atoi(argv[4]);
		}
		else if(strcmp(argv[3],"-i")==0 && strcmp(argv[1],"-p")==0)
		{
			strcpy(siip,argv[2]);
			sipt=atoi(argv[4]);
		}
		else 
		{
			printf("ERROR: wrong application usage \n");
			printf("rmb [-i siip] [-p sipt]\n");
			exit(0);
		}	
	}
	else
	{
		siPortIp(siip,&sipt);	
		printf("identity server ip: %s \n",siip);
		printf("identity server port: %d \n",sipt);		 
	}

	myFd = udpConnect();
	if(myFd == -1)
		printf("ERROR in udp connection");
  
	// mandar mensagem para ir buscar os servidores
	printf("Enter a command:  ");
	while(1){
		keyboardRead(siip,sipt);
	}

  return 1;
}
