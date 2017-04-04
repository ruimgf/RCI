#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MESSAGESIZE 141

typedef struct fdNode_{
    int fd;
    struct fdNode_ * next;
}fdNode;

typedef struct fdList_{
    struct fdNode_ * begin;
    struct fdNode_ * end;
}fdList;

fdList  *  createFdList();
void insertFdListEnd(fdList * m,int fd);
void removeFdListEnd(fdList * m,int fd);
void freeFdList(fdList * m);
int FdListLen(fdList * m);
int getNFd(fdList * m, int n);
