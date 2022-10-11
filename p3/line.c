/**
  */

#include "line.h"


int measureLine(FILE *fp) {
//  *fp = fopen(fp, "r");
  char ch;
  fscanf(fp, "%c", &ch);
  int count = 0;
  if (ch == '\n') {
    count++;                //Newline is first char case.
  }
  while (ch != '\n') {
    if (ch == EOF) {
      break;                //No more input case, dont add to count
    } 
    count++;
    fscanf(fp, "%c", &ch);
    if (ch == '\n') {
      count++;             //If at newLine char case
    }
  }
  fseek(fp, -count, SEEK_CUR);
  return count;
}


void readLine(FILE *fp, char str[]) {
  int len = sizeof(str) / sizeof(char);
  char ch;
  for (int i = 0; i < len; i++) {
    fscanf(fp, "%c", &ch);
    str[i] = ch;
  }
  
//  printf("%c\n", str[0]);
}

