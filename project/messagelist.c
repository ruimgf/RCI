#include "messagelist.h"


void createMessageList(messageList ** begin,messageList ** end){
    *begin = NULL;
    *end = NULL;
}

void insertMessageListEnd(messageList ** begin,messageList ** end, char * message, int lc){
    messageList * aux;
    messageList * insertItem;
    aux = *end;


    insertItem = (messageList *)malloc(sizeof(messageList));
    strcpy(insertItem->message,message);
    insertItem->lc = lc;
    insertItem->next = NULL;

    if(*begin == NULL){ // Empty List
      *begin = insertItem;
      insertItem->prev == NULL;
      *end = insertItem;
    }else{
      aux->next = insertItem;
      insertItem->prev = aux;
      *end = insertItem;
    }
}

void printMessageList(messageList * begin){
    messageList * aux;
    aux = begin;
    while(aux != NULL){
        printf("%s",aux->message);
        aux = aux->next;
    }
}

char * getLastNmessages(messageList * end, int n){
      messageList * aux = end;
      char * ret;
      char str[150];
      ret = malloc((9 + 141 * n) * sizeof(char));
      int i;
      printf("1\n");
      for( i = 0 ; i < n - 1 ; i++){
          if(aux->prev==NULL){
            break;
          }
          aux=aux->prev;
      }
      printf("2\n");
      sprintf(ret,"MESSAGES\n");
      while(aux != NULL){
          sprintf(str,"%s\n",aux->message);
          strcat(ret,str);
          aux = aux->next;
      }

      return ret;


}
