/**
 *
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *readLine(FILE *fp) {    //TESTED SHOULD BE GOOD
  int len = 0;
  char ch = fgetc(fp);
  while (ch != EOF) {
    if (ch == '\n') {
      len++;
      break;
    }
    ch = fgetc(fp);
    len++;
  }

  char *str = (char *)malloc(len);
  str[0] = '\0';
  ch = '\0';
  fseek(fp, -len, SEEK_CUR);
  for (int i = 0; i < len; i++) {
    ch = fgetc(fp);
    str[i] = ch;
  }
  return str;
}

