#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>	
#include <time.h>
#include "udp.h"

#define REFRESH_RATE 5
#define BUFFERSIZE 3000
#define FIELD_SEP ';'
#define LINE_SEP '\n'

// global variables
int myFd;
//int msgservPort = 2115;
//char msgservIp[] = "127.0.0.1";
char siip[16];
int sipt;
char buffer[BUFFERSIZE];
int num_msgservs =0;

typedef struct msgserv_{
	char ip[16];    //endereço IP
	int upt;        //port udp
	int tpt;        //port tcp
}msgserv;

msgserv msgservers[100];




////////////////////////////// HELP ///////////////////////////////////
void help(){

	printf(" RMB APP COMMANDS\n");
	printf(" show_servers             -identities of registed message servers \n");
	printf(" publish message          -text message publication with 140 characters maximum \n");
	printf(" show_lastest_messages n  -last n messages saved in message servers \n");
	printf(" exit                     -application exit\n");
}

////////////////////////////// publishMessage /////////////////////////
void publishMessage(char* message, int r)
{
    udpWriteTo(myFd,message,strlen(message),msgservers[r].ip, msgservers[r].upt);
    // add here code to send message to server
}

////////////////////////////// getServers//////////////////////////////
void getServers()
{
	int len=0;
	char port_udp[10],port_tcp[10];
	char * str, * str2;	
	int nread;

	if ((len=udpWriteTo(myFd, "GET_SERVERS", 11, siip, sipt)) >= 0)
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

////////////////////////////// keyboardRead  //////////////////////////
void keyboardRead(int r)
{
	char command[30];
	char message[140];
	int nread=0;
	size_t ln = 0;

	if(fgets(buffer, BUFFERSIZE , stdin) != NULL)
	{
		// Retirar \n da linha lida
		ln = strlen(buffer)-1;
		if (buffer[ln] == '\n')
			buffer[ln] = '\0';
		if(ln == 0)
		{
			printf("Unkown command\n");
			help();
			return;
		}
		
		sscanf(buffer,"%s",command);

		if(strcmp("show_servers",command)==0)
		{
			printf("Show Servers\n");
			getServers(msgservers);
		}
		else if(strcmp("publish",command)==0)
		{
			strncpy(message, buffer+8, 140);
			sprintf(buffer,"PUBLISH %s",message);
			publishMessage(buffer,r);
		}
		else if(strcmp("show_lastest_messages",command)==0)
		{
			int n,len;
			printf("Show mensagens\n");
			sscanf(buffer,"%s s%d",command,&n);
			sprintf(buffer,"GET_MESSAGES %d",n);
			len=strlen(buffer);	
			
			if (udpWriteTo(myFd, buffer, len, msgservers[r].ip , msgservers[r].upt) < 0)
			{
				printf("udp didint write");
				return;
			}
			printf ("UDP WRITE BYTES: %d\n", len);

			struct timeval tr;
			fd_set rfds;
			time_t select_ini, select_end;
			tr.tv_usec = 0;
			select_ini = time(0);
			select_end = select_ini;
			tr.tv_sec = REFRESH_RATE;
			
			FD_ZERO(&rfds);
			FD_SET(myFd,&rfds);
			FD_SET(1,&rfds);
			int counter;

			counter=select(myFd+1,&rfds,(fd_set*)NULL,(fd_set*)NULL,&tr);
			select_end = time(0);
		
			if(select_end - select_ini >= REFRESH_RATE)
			{	
				printf("udp didint send");
				return;
			}
			else
			{				
				if(counter < 0)
					return;
				else if(counter > 0 )
				{
					printf("Enter a command:  ");
					if(FD_ISSET(myFd,&rfds))
					{
						nread=udpRead(myFd, buffer, BUFFERSIZE);
						if (nread >= 0)
							write(1,buffer,nread);
					}
					else if(FD_ISSET(1,&rfds))
					{
						fgets(buffer, BUFFERSIZE , stdin);
					}
				}				
			}
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
}

void siPortIp()
{
	struct hostent *serverid;
	struct in_addr *serverid_ip;
	if((serverid=gethostbyname("tejo.tecnico.ulisboa.pt"))==NULL)
		exit(1);
	serverid_ip=(struct in_addr*)serverid->h_addr_list[0];

	sprintf(siip,"%s",inet_ntoa(*serverid_ip));
	sipt = 59000;
}


int main(int argc, char ** argv)
{
	srand(time(NULL)); 

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
		siPortIp();
		printf("identity server ip: %s \n",siip);
		printf("identity server port: %d \n",sipt);
	}

	myFd = udpSockect();
	if(myFd == -1)
		printf("ERROR in udp socket");

	// mandar mensagem para ir buscar os servidores
	getServers();
	int r = rand()%num_msgservs;      // returns a pseudo-random integer between 0 and RAND_MAX
	
	while(1)
	{
		printf("Enter a command:  ");
		keyboardRead(r);
	}

  return 1;
}
