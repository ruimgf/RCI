#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MESSAGESIZE 140

typedef struct messageNode_{
    char message[MESSAGESIZE]; // matrix of messages
    int lc; //
    struct messageNode_ * next;
    struct messageNode_ * prev;
}messageNode;

typedef struct messageList_{
    struct messageNode_ * begin;
    struct messageNode_ * end;
}messageList;

messageList  *  createMessageList();
void insertMessageListEnd(messageList * m, char * message, int lc);
char * getLastNmessages(messageList * m, int n);
void printMessageList(messageList * m);
void freeMessageList(messageList * m);
