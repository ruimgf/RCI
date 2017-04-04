#include "messagelist.h"

int max_1(int x, int y){
    if(x>y)
      return x;
    return y;
}

messageList  * createMessageList(int max){
    messageList  * m1 = malloc(sizeof(messageList));
    m1->actualLc = 0;
    m1->begin = NULL;
    m1->end = NULL;
    m1->nmessages=0;
    m1->maxmessages = max;
    return m1;
}

void insertMessageListEnd(messageList * m , char * message, int lc){
    messageNode * aux;
    messageNode * insertItem;
    aux = m->end;
    int int_lc;
    m->nmessages++;
    if(m->nmessages>m->maxmessages){
      m->nmessages = m->maxmessages;
      aux = m->begin;
      m->begin = m->begin->next;
      free(aux);
    }

    if(lc == -1){
      int_lc = m->actualLc;
      m->actualLc++;
    }else{
      m->actualLc = max_1(m->actualLc,lc) + 1;
      int_lc = lc;
    }

    insertItem = (messageNode *)malloc(sizeof(messageNode));

    strcpy(insertItem->message,message);
    insertItem->lc = int_lc;
    insertItem->next = NULL;


    if(m->begin == NULL){ // Empty List

      m->begin = insertItem;
      insertItem->prev = NULL;
      m->end = insertItem;
    }else{
      if(m->end->lc <= int_lc){
        m->end->next = insertItem;
        insertItem->prev = m->end;
        m->end = insertItem;
        return;
      }
      aux=m->begin;
      while(aux!=NULL){
        if(aux->lc > int_lc){
            insertItem->next = aux;
            aux->prev = insertItem;
            return;
        }
        aux = aux->next;
      }
    }
}

void printMessageList(messageList * m){
    messageNode * aux;
    aux = m->begin;
    if(aux==NULL)
      printf("Lista vazia\n");
    while(aux != NULL){
        printf("%s %d\n",aux->message,aux->lc);
        aux = aux->next;
    }
}

char * getLastNmessages(messageList * m, int n){
      messageNode * aux = m->end;
      char * ret;
      char str[150];
      ret = malloc((9 + 141 * n) * sizeof(char));
      int i;

      for( i = 0 ; i < n - 1 ; i++){
          if(aux->prev==NULL){
            break;
          }
          aux=aux->prev;
      }

      sprintf(ret,"MESSAGES\n");
      while(aux != NULL){
          sprintf(str,"%s\n",aux->message);
          strcat(ret,str);
          aux = aux->next;
      }

      return ret;
}

void freeMessageList(messageList * m){
    messageNode * aux = m->begin;
    messageNode * aux1;

    while(aux != NULL){
      aux1 = aux-> next;
      free(aux);
      aux = aux1;
    }
    free(m);
}

char * getAllMessages(messageList * m){
  messageNode * aux = m->begin;
  char * ret;
  char str[300];
  ret = malloc((9 + 141 * 400) * sizeof(char));

  sprintf(ret,"SMESSAGES\n");
  while(aux != NULL){
      sprintf(str,"%d;%s\n",aux->lc,aux->message);
      strcat(ret,str);
      aux = aux->next;
  }
  sprintf(str,"\n");
  strcat(ret,str);
  return ret;

}

void saveMessages(messageList * m, char * messages){

   char * curLine = messages;
   int parse_lc;
   char parse_message[160];
   char strlc[10];
   char * nextLine;
   char * begin_string;

   curLine = strchr(curLine, '\n');
   int l = strlen(messages);
   if(messages[l]!='\n'&&messages[l-1]!='\n'){
     printf("mensagem mal recebida\n");
     return;
   }
   if(strlen(messages)<12)
      return;

   curLine++; // skipe SMESSAGES
   while(curLine)
   {

      nextLine = strchr(curLine, '\n');

      if (nextLine){
        *nextLine = '\0';  // temporarily terminate the current line
        //sscanf(curLine,"%d;%s",&parse_lc,parse_message);
        begin_string = strchr(curLine, ';');

        strncpy(strlc,curLine,(begin_string - curLine));
        parse_lc = atoi(strlc);
        parse_message[begin_string - curLine]='\0';
        begin_string ++;
        strncpy(parse_message,begin_string,(nextLine - begin_string));
        parse_message[nextLine - begin_string]='\0';

        insertMessageListEnd(m,parse_message,parse_lc);
      }

      if(*(nextLine+1)=='\n'){
        return;// end
      }
      curLine=nextLine+1;
   }
}
