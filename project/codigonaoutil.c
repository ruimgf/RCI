messageBox * m;

typedef struct messageBox_{
    int actual_pos; // actual first free position
    char ** messages; // matrix of messages
    int length; // max length off box
}messageBox;

messageBox * createMessageBox(int length){
      messageBox * m;
      int i;
      m = malloc(sizeof(messageBox));
      m->actual_pos = 0;
      m->length = length;
      m->messages = malloc(length * sizeof(char *));
      for(i=0;i<MESSAGESIZE;i++){
        m->messages[i] = malloc(MESSAGESIZE * sizeof(char));
      }
      return m;
}

void saveMessage(messageBox * m ,char * message){
     sprintf(m->messages[m->actual_pos],"%s",message);
     m->actual_pos = m->actual_pos + 1;
}

void printAllMessages(messageBox * m){
    int i;
    // cuidado se ja deu a volta ou nao
    for (i = 0; i < m->actual_pos; i++) {
      printf("%s",m->messages[i]);
    }
}

void destructMessageBox(messageBox * m){
    int i;
    for ( i = 0; i < m->length; i++) {
      free(m->messages[i]);
    }
    free(m->messages);
    free(m);
}
