#include "messagelist.h"


messageList  * createMessageList(){
    messageList  * m1 = malloc(sizeof(messageList));
    m1->actualLc = 0;
    m1->begin = NULL;
    m1->end = NULL;
    return m1;
}

void insertMessageListEnd(messageList * m , char * message, int lc){
    messageNode * aux;
    messageNode * insertItem;
    aux = m->end;


    insertItem = (messageNode *)malloc(sizeof(messageNode));

    strcpy(insertItem->message,message);
    insertItem->lc = lc;
    insertItem->next = NULL;


    if(m->begin == NULL){ // Empty List

      m->begin = insertItem;
      insertItem->prev = NULL;
      m->end = insertItem;
    }else{
      aux->next = insertItem;
      insertItem->prev = aux;
      m->end = insertItem;
    }
}

void printMessageList(messageList * m){
    messageNode * aux;
    aux = m->begin;
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
   char * nextLine;
   curLine = strchr(curLine, '\n');
   curLine++; // skipe SMESSAGES
   while(curLine)
   {
      nextLine = strchr(curLine, '\n');
      if (nextLine){
        *nextLine = '\0';  // temporarily terminate the current line
        sscanf(curLine,"%d;%s",&parse_lc,parse_message);
        insertMessageListEnd(m,parse_message,parse_lc);
      }

      if(*(nextLine+1)=='\n'){
        return;// end
      }
      curLine=nextLine+1;
   }
}
