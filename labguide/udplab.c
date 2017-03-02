//test.c
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
int main(void)
{
int fd, n, addrlen;
struct sockaddr_in addr;
struct in_addr iaddr;
char buffer[128];

inet_aton("193.136.138.142",&iaddr);
fd=socket(AF_INET,SOCK_DGRAM,0);//UDP socket
if(fd==-1)
  exit(1);//error

memset((void*)&addr,(int)'\0',sizeof(addr));
addr.sin_family=AF_INET;
addr.sin_addr=iaddr;
addr.sin_port=htons(58000);
n=sendto(fd,"Hello cris!\n",12,0,(struct sockaddr*)&addr,sizeof(addr));

if(n==-1)
  exit(1);//error

addrlen=sizeof(addr);
n=recvfrom(fd,buffer,128,0,(struct sockaddr*)&addr,&addrlen);
if(n==-1)
  exit(1);

write(1,"echo: ",6);
write(1,buffer,n);

close(fd);
exit(0);
}
