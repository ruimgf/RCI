#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "udp.h"

#define BUFFERSIZE 3000
#define FIELD_SEP ';'
#define LINE_SEP '\n'

// global variables
int myFd;
int msgservPort = 2115;
char msgservIp[] = "127.0.0.1";


typedef struct msgserv_{
	char ip[16];    //endereço IP
	int upt;        //port udp
	int tpt;        //port tcp
}msgserv;


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
	char command[30];
	char message[140];
	int nread=0;
	int len=0;
	char port[10];
	char * str, * str2;

	msgserv msgservers[100];

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

			if ((len=udpWriteTo(myFd, "GET_SERVERS", 11, siip, siport)) >= 0)
			{
				printf ("UDP WRITE BYTES: %d\n", len);
				nread=udpRead(myFd, buffer, BUFFERSIZE);

				if (nread >= 0)
				{
					printf("go clean");
					//write(1,buffer,nread);
					buffer[nread] = '\0';
					printf("NREAD: %d, BUFFER: %s", nread, buffer);

					//descarta o SERVERS
					str=strchr(buffer,LINE_SEP);
					printf("%s", str);


					int i=0;
					while((str=strchr(str,LINE_SEP))!=NULL)
					{
					printf ("WHILE\n");
						// Descarta o nome


						str = strchr(str,FIELD_SEP);
						if(str == NULL) // Fim de tudo
							break;

						str++;

						// IP
						str2 = strchr(str,FIELD_SEP);
						
						if(str2  == NULL) // Mensagem mal definida
							break;

						len = strlen(str)-strlen(str2);
						strncpy(msgservers[i].ip,str,len);
						msgservers[i].ip[len] = '\0';

						// UDP
						str=++str2;
						str2=strchr(str,FIELD_SEP);
						if(str2 == NULL) // Mensagem mal definida
							break;
						len = strlen(str)-strlen(str2);
						strncpy(port,str,len);
						port[len]='\0';
						msgservers[i].upt = atoi(port);

						// TCP
						str=++str2;
						str2=strchr(str,LINE_SEP);
						if(str2 == NULL) // Mensagem mal definida
							break;
						len = strlen(str)-strlen(str2);
						strncpy(port,str,len);
						port[len]='\0';
						msgservers[i].tpt = atoi(port);

						printf("Servidor %d - IP: %s,\t UDP: %d,\t TCP: %d\n",i,msgservers[i].ip,msgservers[i].upt,msgservers[i].tpt);
						i++;
					}
				}
			}
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
