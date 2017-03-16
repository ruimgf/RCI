#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MESSAGESIZE 140

typedef struct messageList_{
    char message[MESSAGESIZE]; // matrix of messages
    int lc; //
    struct messageList_ * next;
}messageList;

messageList * createMessageList();
messageList * insertMessageListEnd(messageList * begin, char * message, int lc);
void printMessageList(messageList * begin);
