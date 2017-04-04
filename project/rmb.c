
#include "geral.h"

/* ------------------ GLOBAL VARIABLES ------------------------------ */
int myFd;                 //sockect
char siip[16];            //IP server of identities
int sipt;                 //PORT server of identities
char buffer[BUFFERSIZE];  //Buffer to comunicate - read and write
int num_msgservs =0;      //Number of servers in the network

msgserv msgservers[100];

/* ------------------ HELP FUNCTION --------------------------------- */
void help()
{
	printf("\n RMB APP COMMANDS\n");
	printf(" show_servers             -identities of registed message servers \n");
	printf(" publish message          -text message publication with 140 characters maximum \n");
	printf(" show_latest_messages n  -last n messages saved in message servers \n");
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

///////////////////////// show last messages /////////////////////////

void showLastMessages (char * command, int n_server)
{
	int n,len, nread;
	struct timeval tr;
	fd_set rfds;
	tr.tv_sec = REFRESH_RATE;

	FD_ZERO(&rfds);
	FD_SET(myFd,&rfds);

	sscanf(buffer,"%s %d",command,&n);
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
			getServers(myFd,msgservers,&num_msgservs,siip,sipt);
			printServers(msgservers,num_msgservs);
		}
		else if(strcmp("publish",command)==0)
		{
			strncpy(message, buffer+8, 140);
			sprintf(buffer,"PUBLISH %s",message);
			publishMessage(buffer,random_server);
		}
		else if(strcmp("show_latest_messages",command)==0)
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
  getServers(myFd,msgservers,&num_msgservs,siip,sipt);
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
