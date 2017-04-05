#include "messagelist.h"
/**
 * [max_1 ]
 * @param  x [description]
 * @param  y [description]
 * @return   [return max between x and y]
 */
int max_1(int x, int y){
    if(x>y)
      return x;
    return y;
}

/**
 * [createMessageList createMessageList]
 * @param  max [max number of messages]
 * @return
 */
messageList  * createMessageList(int max){
    messageList  * m1 = malloc(sizeof(messageList));
    m1->actualLc = 0;
    m1->begin = NULL;
    m1->end = NULL;
    m1->nmessages=0;
    m1->maxmessages = max;
    return m1;
}
/**
 * [insertMessageListEnd insert on the end off list]
 * @param m       [pointer to list]
 * @param message [message to save]
 * @param lc      [logical clock]
 */
void insertMessageListEnd(messageList * m , char * message, int lc){
    messageNode * aux;
    messageNode * insertItem;
    aux = m->end;
    int int_lc;

    m->nmessages++;
    if(m->nmessages > m->maxmessages){
      m->nmessages = m->maxmessages;
      aux = m->begin;
      m->begin = m->begin->next;
      m->begin->prev = NULL;
      free(aux);
    }

    if(lc == -1){
      int_lc = m->actualLc;
      m->actualLc++;
    }else{
      m->actualLc = max_1(m->actualLc,lc) + 1;
      int_lc = lc;
    }

    insertItem = (messageNode *)malloc(sizeof(messageNode));
    if(insertItem == NULL){
      printf("ERROR : malloc\n");
      exit(-1);
    }
    strcpy(insertItem->message,message);
    insertItem->lc = int_lc;
    insertItem->next = NULL;


    if(m->begin == NULL){ // Empty List
      m->begin = insertItem;
      insertItem->prev = NULL;
      m->end = insertItem;
    }else{
      if(m->end->lc <= int_lc){
        m->end->next = insertItem;
        insertItem->prev = m->end;
        m->end = insertItem;
        return;
      }
      aux=m->begin;
      while(aux!=NULL){
        if(aux->lc > int_lc){
            insertItem->next = aux;
            aux->prev = insertItem;
            return;
        }
        aux = aux->next;
      }
    }

}

/**
 * [printMessageList print list]
 * @param m [pointer to messageList]
 */
void printMessageList(messageList * m){
    messageNode * aux;
    aux = m->begin;
    if(aux==NULL)
      printf("Lista vazia\n");
    while(aux != NULL){
        printf("%d : %s\n",aux->lc,aux->message);
        aux = aux->next;
    }
}

/**
 * [getLastNmessages description]
 * @param  m [description]
 * @param  n [number of messages]
 * @return   [pointer to string contain all messages]
 */
char * getLastNmessages(messageList * m, int n){
      messageNode * aux = m->end;
      char * ret;
      char str[150];
      ret = malloc((9 + 141 * n) * sizeof(char));
      if(ret == NULL){
        printf("ERROR : malloc\n");
        exit(-1);
      }
      int i;

      for( i = 0 ; i < n - 1 ; i++){
          if(aux->prev==NULL){
            break;
          }
          aux=aux->prev;
      }

      sprintf(ret,"MESSAGES\n");
      while(aux != NULL){
          sprintf(str,"%s\n",aux->message);
          strcat(ret,str);
          aux = aux->next;
      }

      return ret;
}
/**
 * [freeMessageList free messageList]
 * @param m [pointer to messageList]
 */
void freeMessageList(messageList * m){
    messageNode * aux = m->begin;
    messageNode * aux1;

    while(aux != NULL){
      aux1 = aux-> next;
      free(aux);
      aux = aux1;
    }
    free(m);
}
/**
 * [getAllMessages return all messages]
 * @param  m [pointer to message List]
 * @return   [char *  from string with all messages]
 */
char * getAllMessages(messageList * m){
  messageNode * aux = m->begin;
  char * ret;
  char str[300];
  ret = malloc((9 + 141 * 400) * sizeof(char));
  if(ret == NULL){
    printf("ERROR : malloc\n");
    exit(-1);
  }
  sprintf(ret,"SMESSAGES\n");
  while(aux != NULL){
      sprintf(str,"%d;%s\n",aux->lc,aux->message);
      strcat(ret,str);
      aux = aux->next;
  }
  sprintf(str,"\n");
  strcat(ret,str);
  return ret;

}
/**
 * [saveMessages Save all messages form a string]
 * @param  m        [pointer to messageList]
 * @param  messages [all messages]
 * @return          [status]
 */
int saveMessages(messageList * m, char * messages){

   char * curLine = messages;
   int parse_lc;
   char parse_message[160];
   char strlc[10];
   char * nextLine;
   char * begin_string;

   curLine = strchr(curLine, '\n');
   int l = strlen(messages);
   if(messages[l]!='\n'&&messages[l-1]!='\n'){
     return -1;
   }
   if(strlen(messages)<12)
      return -1;

   curLine++; // skipe SMESSAGES
   while(curLine)
   {

      nextLine = strchr(curLine, '\n');

      if (nextLine){
        *nextLine = '\0';  // temporarily terminate the current line
        //sscanf(curLine,"%d;%s",&parse_lc,parse_message);
        begin_string = strchr(curLine, ';');
          if(begin_string!=NULL){
          strncpy(strlc,curLine,(begin_string - curLine));
          parse_lc = atoi(strlc);
          parse_message[begin_string - curLine]='\0';
          begin_string ++;
          strncpy(parse_message,begin_string,(nextLine - begin_string));
          parse_message[nextLine - begin_string]='\0';

          insertMessageListEnd(m,parse_message,parse_lc);
        }
      }

      if(*(nextLine+1)=='\n'){
        return 0;// end
      }
      curLine=nextLine+1;
   }
   return 0;
}
