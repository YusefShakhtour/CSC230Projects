/**
 *
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *readLine(FILE *fp) {    //TESTED SHOULD BE GOOD
  int len = 0;
//  fscanf(fp, "%[^\n]%*c%n", line);
  char ch = fgetc(fp);
//  printf("%s%c", "First char: ", ch);
//  ch = fgetc(fp);
//  printf("%s%c", "Next char: ", ch);

  if (ch == EOF) {
    return "NULL"; //not string NULL
  }
  while (ch != EOF) {
    if (ch == '\n') {    //Store char at a time in str, if needs to be expanded, double capacity
      len++;
      break;
    }
    ch = fgetc(fp);
    len++;
  }
//  printf("%s%d","LENGHT", len);
//Fomr here on is garbage
  char *str = (char *)malloc(len);   //Dont need to count lenght of line
  str[0] = '\0';
  ch = '\0';
  fseek(fp, -len, SEEK_CUR);
//  int count = 0;
  for (int i = 0; i < len; i++) {
    ch = fgetc(fp);
    str[i] = ch;
//    count++;
  }
//  str[count - 1] = '\0';
  return str;
}

