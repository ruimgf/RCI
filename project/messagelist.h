#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MESSAGESIZE 141
// struct
typedef struct messageNode_{
    char message[MESSAGESIZE]; // matrix of messages
    int lc; //
    struct messageNode_ * next;
    struct messageNode_ * prev;
}messageNode;

typedef struct messageList_{
    int actualLc;
    struct messageNode_ * begin;
    struct messageNode_ * end;
    int nmessages;
    int maxmessages;
}messageList;

/**
 * [createMessageList createMessageList]
 * @param  max [max number of messages]
 * @return
 */
messageList  *  createMessageList(int max);
/**
 * [insertMessageListEnd insert on the end off list]
 * @param m       [pointer to list]
 * @param message [message to save]
 * @param lc      [logical clock]
 */
void insertMessageListEnd(messageList * m, char * message, int lc);
/**
 * [getLastNmessages description]
 * @param  m [description]
 * @param  n [number of messages]
 * @return   [pointer to string contain all messages]
 */
char * getLastNmessages(messageList * m, int n);
/**
 * [getAllMessages all messages]
 * @param  m [pointer to messageList]
 * @return   [string to string]
 */
char * getAllMessages(messageList * m);
/**
 * [printMessageList print list]
 * @param m [pointer to messageList]
 */
void printMessageList(messageList * m);
/**
 * [freeMessageList free messageList]
 * @param m [pointer to messageList]
 */
void freeMessageList(messageList * m);
/**
 * [saveMessages Save all messages form a string]
 * @param  m        [pointer to messageList]
 * @param  messages [all messages]
 * @return          [status]
 */
int saveMessages(messageList * m, char * messages);
