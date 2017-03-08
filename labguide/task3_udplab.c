//test.c
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
int main(void)
{
int fd, n1, n2, addrlen;
struct sockaddr_in addr;
struct in_addr iaddr;
char buffer[128];
//char buffer2[128];


inet_aton("193.136.138.142",&iaddr);
fd=socket(AF_INET,SOCK_DGRAM,0);//UDP socket
if(fd==-1)
  exit(1);//error

memset((void*)&addr,(int)'\0',sizeof(addr));
addr.sin_family=AF_INET;
addr.sin_addr=iaddr;

addr.sin_port=htons(58000);
n1=sendto(fd,"Hello cris!\n",12,0,(struct sockaddr*)&addr,sizeof(addr));
n2=sendto(fd,"Hello!\n",7,0,(struct sockaddr*)&addr,sizeof(addr));

if(n1==-1)
  exit(1);//error

if(n2==-1)
  exit(1);//error

addrlen=sizeof(addr);

n1=recvfrom(fd,buffer,128,0,(struct sockaddr*)&addr,&addrlen);
write(1,"echo: ",6);
write(1,buffer,n1);


if(n1==-1)
  exit(1);
n2=recvfrom(fd,buffer,128,0,(struct sockaddr*)&addr,&addrlen);
if(n2==-1)
  exit(1);

write(1,"echo: ",6);
write(1,buffer,n2);

close(fd);
exit(0);
}
