#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "udp.h"


#define REFRESH_RATE 5
#define FIELD_SEP ';'
#define LINE_SEP '\n'
#define BUFFERSIZE 10000

/* ------------------   MESSAGE SERVERS    -------------------------- */
typedef struct msgserv_{
  char name[20];  //name
	char ip[16];    //IP address
	int upt;        //UDP port
	int tpt;        //TCP port
}msgserv;

typedef struct appSpec_{
	char name[20];    //endereço IP
	char ip[16];      //endereço IP
  int upt;          //port udp
	int tpt;          //port tcp
  char siip[16];    //IP do servidor de identidades
  int sipt;         //porto do servidor de identidades
  int m;            //nmr max de msg guardadas
  int r;            //time out
}appSpec;


void getServers(int myFd ,msgserv msgservers[100],int  * num_msg, char * siip, int sipt);
void printServers(msgserv msgservers[100], int num_msgservs);
void siPortIp(char * siip, int * sipt);
