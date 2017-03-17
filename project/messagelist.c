#include "messagelist.h"


messageList  * createMessageList(){
    messageList  * m1 = malloc(sizeof(messageList));
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
        printf("%s",aux->message);
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
