#include "geral.h"


/**
 * [getServers get list of servers from identities server]
 * @param myFd    [udp descriptor of  requester]
 * @param [msgserv]  [array to save msg servers]
 * @param num_msg [number of msg servers]
 * @param siip    [ip of identities server]
 * @param sipt    [port of identities server]
 */
void getServers(int myFd ,msgserv msgservers[100],int  * num_msg, char * siip, int sipt)
{
  char buffer[BUFFERSIZE];
	char port_udp[10], port_tcp[10];
	char * str, * str2;
	int nread = 0;
  int len = 0;
  int num_msgservs=0;

  *num_msg  = 0;
  struct timeval tr;
	fd_set rfds;
	tr.tv_sec = REFRESH_RATE;

	FD_ZERO(&rfds);
	FD_SET(myFd,&rfds);


  if (udpWriteTo(myFd, "GET_SERVERS", 11, siip, sipt) < 0)
	{
    printf("ERROR: udp servers didint write\n");
    exit(-1);
    return;
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
    *num_msg  = num_msgservs;
  }
  else
	{
		printf("ERROR: server didint answer\n");
    exit(-1);
	}
}
/**
 * [printServers print information of message servers]
 * @param [name]       [array of msgservers]
 * @param num_msgservs [number of msgservers]
 */
void printServers(msgserv msgservers[100], int num_msgservs){
  int i;
  if(num_msgservs==0)
      printf("no servers available\n");
  for( i=0;i<num_msgservs;i++)
  {
    printf("Servidor %d - NAME: %s\t IP: %s\t UDP: %d\t TCP: %d\n",
          i, msgservers[i].name,msgservers[i].ip,msgservers[i].upt,
          msgservers[i].tpt);
  }

}
/**
* [siPortIp get ip from defaul si tejo.tecnico.ulisboa.pt]
* @param siip [description]
* @param sipt [description]
*/
void siPortIp(char * siip, int * sipt)
{
	struct hostent *serverid;
	struct in_addr *serverid_ip;
	if((serverid=gethostbyname("tejo.tecnico.ulisboa.pt"))==NULL)
		exit(1);

	serverid_ip=(struct in_addr*)serverid->h_addr_list[0];

	sprintf(siip,"%s",inet_ntoa(*serverid_ip));
	*sipt = 59000;


}
