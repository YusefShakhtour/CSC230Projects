/**
 *
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *readLine(FILE *fp) {    //TESTED SHOULD BE GOOD
  int cap = 10;
  char *str = (char *)malloc(cap);
  char ch = fgetc(fp);

  if (ch == EOF) {
    free(str);
    return NULL; //not string NULL
  }
  int count = 0;
  while (ch != EOF) {
    if (ch == '\n') {    //Store char at a time in str, if needs to be expanded, double capacity
      break;
    }
    if (strlen(str) >= cap) {
      cap = cap * 2;
      str = (char *)realloc(str, sizeof(char) * cap);
    }
    str[count] = ch;
    count++;
    ch = fgetc(fp);
  }
    str[count] = '\0';
  return str;
}

