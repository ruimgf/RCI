#include "fdlist.h"


fdList  * createFdList(){
    fdList  * m1 = malloc(sizeof(fdList));
    m1->begin = NULL;
    m1->end = NULL;
    return m1;
}

void insertFdListEnd(fdList * m ,int fd){
    fdNode * aux;
    fdNode * insertItem;
    aux = m->end;


    insertItem = (fdNode *)malloc(sizeof(fdNode));


    insertItem->fd = fd;



    if(m->begin == NULL){ // Empty List
      m->begin = insertItem;

      m->end = insertItem;
    }else{
      aux->next = insertItem;

      m->end = insertItem;
    }
}

void freeFdList(fdList * m){
    fdNode * aux = m->begin;
    fdNode * aux1;

    while(aux != NULL){
      aux1 = aux-> next;
      free(aux);
      aux = aux1;
    }
    free(m);
}
