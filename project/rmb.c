#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include "udp.h"

/* ------------------ DEFINES --------------------------------------- */
#define REFRESH_RATE 5
#define BUFFERSIZE 3000
#define FIELD_SEP ';'
#define LINE_SEP '\n'

/* ------------------ GLOBAL VARIABLES ------------------------------ */
int myFd;                 //sockect
char siip[16];            //IP server of identities
int sipt;                 //PORT server of identities
char buffer[BUFFERSIZE];  //Buffer to comunicate - read and write
int num_msgservs =0;      //Number of servers in the network

/* ------------------ STRUCTURE DEFINITION -------------------------- */
/* ------------------   MESSAGE SERVERS    -------------------------- */

typedef struct msgserv_{
  char name[20];  //name
	char ip[16];    //IP address
	int upt;        //UDP port
	int tpt;        //TCP port
}msgserv;

msgserv msgservers[100];


/* ------------------ HELP FUNCTION --------------------------------- */
void help()
{
	printf("\n RMB APP COMMANDS\n");
	printf(" show_servers             -identities of registed message servers \n");
	printf(" publish message          -text message publication with 140 characters maximum \n");
	printf(" show_lastest_messages n  -last n messages saved in message servers \n");
	printf(" exit                     -application exit\n\n");
}

/* ------------------ PUBLISH MESSAGE FUNCTION ---------------------- */
void publishMessage(char* message, int n_server)
{
  //send through socket myFd "message" to message server number 
  //n_server chosen randomly
  udpWriteTo(myFd,message,strlen(message),msgservers[n_server].ip, 
             msgservers[n_server].upt);
}

/* ------------------ GET SERVERS FUNCTION -------------------------- */
void getServers()
{
	char port_udp[10], port_tcp[10];
	char * str, * str2;
	int nread = 0;
  int len = 0;
  
  struct timeval tr;
	fd_set rfds;
	tr.tv_sec = REFRESH_RATE;
	
	FD_ZERO(&rfds);
	FD_SET(myFd,&rfds);
	

  if (udpWriteTo(myFd, "GET_SERVERS", 11, siip, sipt) < 0)
	{
    printf("ERROR: udp servers didint write\n");
	}

  if(select(myFd+1,&rfds,(fd_set*)NULL,(fd_set*)NULL,&tr))
	{
		nread=udpRead(myFd, buffer, BUFFERSIZE);
  
		if (nread >= 0)
		{
			buffer[nread] = '\0';

			//Descarta o SERVERS
			str2=strchr(buffer,LINE_SEP);
      if(!str2) // Mensagem mal definida
        return;
      
      num_msgservs=0;
			while((str=strchr(str2,LINE_SEP)))
			{
				//NAME
				str = strchr(str2,FIELD_SEP);
        if(!str) // Mensagem mal definida e fim de tudo
					break;

        len = str-str2;
        strncpy(msgservers[num_msgservs].name,str2+1,len-1);
        msgservers[num_msgservs].name[len] = '\0';
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

				num_msgservs++;
			}
		}
  }
  else
	{
		printf("ERROR: server didint answer\n");
	}
}

///////////////////////// show last messages /////////////////////////

void showLastMessages (char * command, int n_server)
{
	int n,len, nread;
	struct timeval tr;
	fd_set rfds;
	tr.tv_sec = REFRESH_RATE;
	
	FD_ZERO(&rfds);
	FD_SET(myFd,&rfds);
	
	sscanf(buffer,"%s s%d",command,&n);
	sprintf(buffer,"GET_MESSAGES %d",n);
	len=strlen(buffer);	
			
	if (udpWriteTo(myFd, buffer, len, msgservers[n_server].ip, 
      msgservers[n_server].upt) < 0)
	{
		printf("ERROR: udp servers didint write\n");
	}
			
	if(select(myFd+1,&rfds,(fd_set*)NULL,(fd_set*)NULL,&tr))
	{
		nread=udpRead(myFd, buffer, BUFFERSIZE);
		if (nread >= 0)
			write(1,buffer,nread);
	}	
	else
	{
		printf("ERROR: server didint answer\n");
	}
}


////////////////////////////// keyboardRead  //////////////////////////
void keyboardRead(int random_server)
{
	char command[30];
	char message[140];
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
		
		if(sscanf(buffer,"%s",command)!=1)
			printf("ERROR:command didint find");

		if(strcmp("show_servers",command)==0)
		{
			getServers(msgservers);
			for(int i=0;i<num_msgservs;i++)
      {
        printf("Servidor %d - NAME: %s\t IP: %s\t UDP: %d\t TCP: %d\n", 
              i, msgservers[i].name,msgservers[i].ip,msgservers[i].upt,
              msgservers[i].tpt);
      }
		}
		else if(strcmp("publish",command)==0)
		{
			strncpy(message, buffer+8, 140);
			sprintf(buffer,"PUBLISH %s",message);
			publishMessage(buffer,random_server);
		}
		else if(strcmp("show_lastest_messages",command)==0)
		{
			showLastMessages(command, random_server);		
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

////////////////////////////// siPortIp  //////////////////////////

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

  // Testes of aplication arguments
	if (argc>1)
	{
    // After a -i or -p 
		if (strcmp(argv[1],"-i")==0 && strcmp(argv[3],"-p")==0)
		{
      // IP of server of identities - put as global variable
			strcpy(siip,argv[2]);
      // PORT of server of identities - put as global variable
      // Cast from *char to int
			sipt=atoi(argv[4]);
		}
    // After a -p or -i 
		else if (strcmp(argv[3],"-i")==0 && strcmp(argv[1],"-p")==0)
		{
      // IP of server of identities - put as global variable
			strcpy(siip,argv[2]);
      // PORT of server of identities - put as global variable
      // Cast from *char to int 
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
    // Default IP and PORT of server of identities
		siPortIp();
		printf("Identities server ip: %s \n",siip);
		printf("Identities server port: %d \n",sipt);
	}

  // Open socket
	myFd = udpSockect();
	if(myFd == -1)
		printf("ERROR in udp socket");

  // Inicialization of message servers available
  getServers();
	if(num_msgservs==0)
	{
		printf("ERROR: there is no server available\n");
		exit(0);
	}
  
  // Random selection of message server to comunicate with
	int random_server = rand()%num_msgservs;      
  
	while(1)
	{
		printf("Enter a command:  ");
		keyboardRead(random_server);
	}
  
  // Close socket
  close(myFd);

  return 1;
}
