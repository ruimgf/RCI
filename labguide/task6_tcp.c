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

  int fd, n;
  int nbytes, nleft, nwritten, nread;
  struct sockaddr_in addr;
  struct in_addr iaddr;
  char buffer[128];
  char *ptr;
  struct hostent *h;

  inet_aton("193.136.138.142",&iaddr);
  fd=socket(AF_INET,SOCK_STREAM,0);//TCP socket
  if(fd==-1)
    exit(1);//error

  memset((void*)&addr,(int)'\0',sizeof(addr));
  addr.sin_family=AF_INET;
  addr.sin_addr=iaddr;
  addr.sin_port=htons(58000);

  void (*old_handler)(int);//interrupt handler

  n=connect(fd,(struct sockaddr*)&addr,sizeof(addr));
  if(n==-1)
  {
    printf("Error do connect");
    exit(1);//error
  }

  if((old_handler=signal(SIGPIPE,SIG_IGN))==SIG_ERR)
    exit(1);//error

  ptr=strcpy(buffer,"Hello\n");
  nbytes=7;
  nleft=nbytes;

  while(nleft>0)
  {
    nwritten=write(fd,ptr,nleft);
    if(nwritten<=0)
      exit(1);//error
    nleft-=nwritten;
    ptr+=nwritten;
  }

  nleft=nbytes;
  ptr=buffer;

  while(nleft>0)
  {
    nread=read(fd,ptr,nleft);
    if(nread==-1)
      exit(1);//error
    else if(nread==0)
      break;//closed by peer
      nleft-=nread;
      ptr+=nread;
  }

  nread=nbytes-nleft;


  write(1,"echo: ",6);//stdout
  write(1,buffer,nread);
  close(fd);

  exit(0);

}
