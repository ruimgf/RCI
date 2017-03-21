#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

appSpec appspec;
 
void wrongUse(){
  printf("Wrong Program Usage : msgserv –n name –j ip -u upt –t tpt [-i siip] [-p sipt] [–m m] [–r r] \n");
  exit(-1);
}

int main(int argc, char *argv[])
{
  char aux[10];
   printf("argc %d\n", argc);
  
  appspec.name[0]='\0';
  appspec.ip[0]='\0';
  appspec.upt=-1;
  appspec.tpt=-1;
  appspec.siip[0]='\0';
  appspec.sipt=-1;
  appspec.m=-1;
  appspec.r=-1;
  
  if(argc < 9 || argc > 17)
  {
    printf("ERRO");
  }
  else
  {
    printf("argc %d\n", argc);
    for(int i=0; i<(argc-1)/2 ; i++)
    { 
      if(strcmp(argv[1+2*i],"-n")==0)     //impares -n
      {
        strcpy(appspec.name, argv[i+1]);
        printf("-n : %d\n", i);
      }
      if(strcmp(argv[1+2*i],"-j")==0)
      {
        strcpy(appspec.ip, argv[i+1]);
        printf("-j : %d\n", i);
      }
      if(strcmp(argv[1+2*i],"-u")==0)
      {
        strcpy(aux, argv[i+1]);
        appspec.upt=atoi(aux);
        printf("-u: %d\n", i);
      }
      if(strcmp(argv[1+2*i],"-t")==0)
      {
        strcpy(aux, argv[i+1]);
        appspec.tpt=atoi(aux); 
        printf("-t: %d\n", i);
      }
      if(strcmp(argv[1+2*i],"-i")==0)
      {
        strcpy(appspec.siip, argv[i+1]);
        printf("-i: %d\n", i);
      }
      if(strcmp(argv[1+2*i],"-p")==0)
      {
        strcpy(aux, argv[i+1]);
        appspec.sipt=atoi(aux);
        printf("-p: %d\n", i);
      }
      if(strcmp(argv[1+2*i],"-m")==0)
      {
        strcpy(aux, argv[i+1]);
        appspec.m=atoi(aux);
        printf("-m: %d\n", i);
      }
      if(strcmp(argv[1+2*i],"-r")==0)
      {
        strcpy(aux, argv[i+1]);
        appspec.m=atoi(aux);
        printf("-r: %d\n", i);
      }
    }
  }
    if(strlen(appspec.name)==0 || strlen(appspec.ip)==0 || appspec.upt==-1 || appspec.tpt==-1)
      wrongUse();
    
  return 0;
}
