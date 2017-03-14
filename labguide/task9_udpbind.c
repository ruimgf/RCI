//Não percebo o que faz o bind aqui!
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
int fd, ret, addrlen, nread,ser;
struct sockaddr_in addr;
struct in_addr iaddr;
char buffer[128];


memset((void*)&addr,(int)'\0',sizeof(addr));
addr.sin_family=AF_INET;
addr.sin_port=htons(9000);
addr.sin_addr.s_addr=htonl(INADDR_ANY);

fd=socket(AF_INET,SOCK_DGRAM,0);//UDP socket
if(fd==-1)
	printf("ERROR IN SOKECT");exit(1);//error

ret=bind(fd,(struct sockaddr*)&addr,sizeof(addr));
if(ret==-1)
	printf("ERROR IN BIND");exit(1);//error
	

ser=inet_aton("193.136.138.142",&iaddr);
if(ser==0)
	printf("ERROR IN PORTO");exit(1);
	
while(1)
{
	addrlen=sizeof(addr);
	nread=recvfrom(fd,buffer,128,0,(struct sockaddr*)&addr,&addrlen);
	if(nread==-1)
		printf("ERROR IN recfrom");exit(1);//error
	ret=sendto(fd,buffer,nread,0,(struct sockaddr*)&addr,addrlen);
	if(ret==-1)
		printf("ERROR IN sendto");exit(1);//error
}

write(1,"echo: ",6);//stdout
write(1,buffer,nread);
close(fd);


exit(0);
}
