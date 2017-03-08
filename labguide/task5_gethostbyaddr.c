#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>

int main(void)
{

  int fd, n, addrlen;
  struct sockaddr_in addr;
  struct in_addr iaddr;
  char buffer[128];
  struct hostent *h;

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

  h=gethostbyaddr(&addr.sin_addr,sizeof(addr.sin_addr),AF_INET);

  if(h==NULL)
    printf("sent by [%s:%hu]\n",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));
  else
    printf("sent by [%s:%hu]\n",h->h_name,ntohs(addr.sin_port));

  exit(0);

}
