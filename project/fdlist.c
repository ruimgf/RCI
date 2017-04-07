#include "fdlist.h"

/**
 * [createFdList create a list of file descriptor]
 * @return [pointer to list]
 */
fdList  * createFdList(){
    fdList  * m1 = malloc(sizeof(fdList));
    if(m1 == NULL){
      printf("ERROR no malloc\n");
      exit(-1);
    }
    m1->begin = NULL;
    m1->end = NULL;
    return m1;
}
/**
 * [insertFdListEnd insert fd on list]
 * @param m  [pointer to fd list]
 * @param fd [fd to insert]
 */
void insertFdListEnd(fdList * m,int fd,char * name, char * ip , int upt, int tpt){
    fdNode * aux;
    fdNode * insertItem;
    aux = m->end;


    insertItem = (fdNode *)malloc(sizeof(fdNode));
    if(insertItem == NULL){
      printf("ERROR MALLOC\n");
      exit(-1);
    }

    insertItem->fd = fd;
    insertItem->tpt = tpt;
    insertItem->upt = upt;
    insertItem->next = NULL;
    strcpy(insertItem->ip,ip);
    strcpy(insertItem->name,name);

    if(m->begin == NULL){ // Empty List
      m->begin = insertItem;

      m->end = insertItem;
    }else{
      aux->next = insertItem;

      m->end = insertItem;
    }
}
/**
 * [removeFdListEnd remove a fd from list]
 * @param m  [pointer to fd list]
 * @param fd [fd to remove]
 */
void removeFdListEnd(fdList * m,int fd){
    fdNode * aux = m->begin;
    fdNode * aux2=m->begin->next;
    if(aux!=NULL){
      if(aux->fd==fd){
        m->begin = aux->next;
        free(aux);
        return;
      }
    }
    while(aux2!=NULL){
      if(aux2->fd==fd){
        aux->next = aux2->next;
        free(aux2);
        return;
      }
      aux2=aux2->next;
      aux=aux->next;
    }
}
/**
 * [FdListLen get fd list len]
 * @param  m [pointer to fd list]
 * @return   [len of list]
 */
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
/**
 * [FdListLen get fd list len]
 * @param  m [pointer to fd list]
 * @return   [len of list]
 */
int FdListLen(fdList * m){

    fdNode * aux = m->begin;
    int i=0;
    while (aux != NULL) {
      i++;
      aux = aux->next;
    }
    return i;
}
/**
 * [getNFd get fd on position n]
 * @param  m [pointer to fd list]
 * @param  n [number of fd to get]
 * @return   [fd]
 */
int getNFd(fdList * m, int n){
    fdNode * aux = m->begin;

    if ( n >= FdListLen(m))
        return -1;

    int i;

    for(i=0; i<n ;i++)
        aux = aux->next;

    return aux->fd;
}


void printFdList(fdList * m){
    fdNode * aux;
    aux = m->begin;
    int i=0;
    if(aux==NULL)
      printf("There are no servers\n");
    while(aux != NULL){
      i++;
      printf("Server %d - NAME: %s\t IP: %s\t UDP: %d\t TCP: %d\n",
            i, aux->name,aux->ip,aux->upt,
            aux->tpt);
        aux = aux->next;
    }
}
