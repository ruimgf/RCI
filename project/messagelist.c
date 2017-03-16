#include "messagelist.h"


messageList * createMessageList(){
    return NULL;
}

messageList * insertMessageListEnd(messageList * begin, char * message, int lc){
    messageList * aux;
    messageList * insertItem;
    aux = begin;


    insertItem = (messageList *)malloc(sizeof(messageList));
    strcpy(insertItem->message,message);
    insertItem->lc = lc;
    insertItem->next = NULL;

    if(begin == NULL){ // Empty List
      return insertItem;
    }else{
      while(aux->next != NULL)
      {
        aux = aux->next;
      }
      aux->next = insertItem;
      return begin;
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
