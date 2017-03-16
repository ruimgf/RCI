#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MESSAGESIZE 140

typedef struct messageList_{
    char message[MESSAGESIZE]; // matrix of messages
    int lc; //
    struct messageList_ * next;
    struct messageList_ * prev;
}messageList;

void createMessageList(messageList ** begin, messageList ** end);
void insertMessageListEnd(messageList ** begin,messageList ** end, char * message, int lc);
char * getLastNmessages(messageList * end, int n);
void printMessageList(messageList * begin);
