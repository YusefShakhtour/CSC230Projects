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
    return NULL; //not string NULL
  }
  int count = 0;
  while (ch != EOF) {
    if (ch == '\n') {    //Store char at a time in str, if needs to be expanded, double capacity
      break;
    }
    if (strlen(str) >= cap) {
      cap = cap * 2;
      str = (char *)realloc(str, cap);
    }
    str[count] = ch;
    count++;
    ch = fgetc(fp);
  }
  //  printf("%d %d\n", count, cap);
    str[count] = '\0';
//  printf("%s%d","LENGHT", len);
//Fomr here on is garbage
//  char *str = (char *)malloc(len);   //Dont need to count lenght of line
//  str[0] = '\0';
//  ch = '\0';
//  fseek(fp, -len, SEEK_CUR);
//  int count = 0;
//  for (int i = 0; i < len; i++) {
//    ch = fgetc(fp);
//    str[i] = ch;
//    count++;
//  str[count - 1] = '\0';
  return str;
}

