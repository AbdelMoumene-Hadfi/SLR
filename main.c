#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct grammar {
  char *first;
  char *second;
  struct grammar *next;
}grammar;
grammar *Grammar_array;
FILE *file;
char *line;
char c;

void push(char* token,char* second) {
  grammar *temp = Grammar_array;
  if(temp == NULL) {
    temp->first = malloc(3);
    memset(temp->first,'\0',3);
    temp->first = strcpy(temp->first,token);
    temp->second = malloc(30);
    memset(temp->second,'\0',30);
    temp->second = strcpy(temp->second,second);
    temp->next = NULL ;
  }
  else {
    while(temp->next != NULL) {
      temp=temp->next;
    }
    temp->next=(grammar*)malloc(sizeof(grammar));
    temp->next->first = malloc(3);
    memset(temp->next->first,'\0',3);
    temp->next->first = strcpy(temp->next->first,token);
    temp->next->second = malloc(30);
    memset(temp->next->second,'\0',30);
    temp->next->second = strcpy(temp->next->second,second);
    temp->next->next = NULL ;
  }
}
void show() {
  grammar *temp = Grammar_array;
  while(temp->next != NULL) {
    printf("%s ->",temp->next->first);
    printf("%s\n",temp->next->second);
    temp=temp->next;
  }
}
void tokinezer() {
  char *first,*token,*token2;
  first = strtok(line,"->");
  token = strtok(NULL,"->");
  token2 = strtok(token,"|");
  while(token2 != NULL) {
    push(first,token2);
    token2 = strtok(NULL,"|");
  }
}
void read_line() {
  memset(line,'\0',50);
  line=strcat(line,&c);
  while((c=fgetc(file)) != '\n') {
    line=strcat(line,&c);
  }
}
int main() {
  file=fopen("test","a+");
  line=malloc(50);
  Grammar_array=(grammar*)malloc(sizeof(grammar));
  while((c=fgetc(file)) != EOF) {
    read_line();
    tokinezer();
  }
  show();
  return 0;
}
