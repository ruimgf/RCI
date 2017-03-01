//test.c
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
int main(void)
{
int fd, n;
struct sockaddr_in addr;
/* ... */
fd=socket(AF_INET,SOCK_DGRAM,0);//UDP socket
if(fd==-1)exit(1);//error
memset((void*)&addr,(int)'\0',sizeof(addr));
addr.sin_family=AF_INET;
addr.sin_addr=
addr.sin_port=htons(8000);
n=sendto(fd,"Hello!\n",7,0,(struct sockaddr*)&addr,sizeof(addr));
if(n==-1)exit(1);//error
/* ... */
}
