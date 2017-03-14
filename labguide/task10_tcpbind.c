#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h> /* for AF_INET */
#include <signal.h>

int main(void)
{

	int fd, addrlen, newfd;
	struct sockaddr_in addr;
	int n, nw;
	char buffer[128];
	char *ptr;
  
	if((fd=socket(AF_INET,SOCK_STREAM,0))==-1)
		exit(1);//error
	
	memset((void*)&addr,(int)'\0',sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=htonl(INADDR_ANY);
	addr.sin_port=htons(58001);
	
	if(bind(fd,(struct sockaddr*)&addr,sizeof(addr))==-1)
		exit(1);//error
		
	if(listen(fd,5)==-1)
		exit(1);//error

	while(1)
	{
		addrlen=sizeof(addr);
		if((newfd=accept(fd,(struct sockaddr*)&addr,&addrlen))==-1)
			exit(1);
		while((n=read(newfd,buffer,128))!=0)
		{
			if(n==-1)
				exit(1);//error
			ptr=&buffer[0];
			while(n>0)
			{
				if((nw=write(newfd,ptr,n))<=0)
					exit(1);//error
				n-=nw;
				ptr+=nw;
			}
		}
			
		close(newfd);	
	}
}
