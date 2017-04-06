#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MESSAGESIZE 141

typedef struct fdNode_{
    int fd;
    char name[20];  //name
    char ip[17];    //IP address
    int upt;        //UDP port
    int tpt;        //TCP port
    struct fdNode_ * next;
}fdNode;

typedef struct fdList_{
    struct fdNode_ * begin;
    struct fdNode_ * end;
}fdList;
/**
 * [createFdList create a list of file descriptor]
 * @return [pointer to list]
 */
fdList  *  createFdList();
/**
 * [insertFdListEnd insert fd on list]
 * @param m  [pointer to fd list]
 * @param fd [fd to insert]
 */
void insertFdListEnd(fdList * m,int fd,char * name, char * ip , int upt, int tpt);
/**
 * [removeFdListEnd remove a fd from list]
 * @param m  [pointer to fd list]
 * @param fd [fd to remove]
 */
void removeFdListEnd(fdList * m,int fd);
/**
 * [freeFdList free list of file descriptor]
 * @param m [description]
 */
void freeFdList(fdList * m);
/**
 * [FdListLen get fd list len]
 * @param  m [pointer to fd list]
 * @return   [len of list]
 */
int FdListLen(fdList * m);
/**
 * [getNFd get fd on position n]
 * @param  m [pointer to fd list]
 * @param  n [number of fd to get]
 * @return   [fd]
 */
int getNFd(fdList * m, int n);

void printFdList(fdList * m);
