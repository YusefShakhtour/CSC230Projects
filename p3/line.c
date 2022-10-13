/**@file line.c
 * @author Yusef Shakhtour (yfshakht)
 * This .c file defines the functions that will be used to read and measure lines in order to properly
 * allocate the necessary memory space for variables
  */

#include "line.h"


int measureLine(FILE *fp) {
  char ch;
  ch = fgetc(fp);
  int count = 0;
  if (ch == '\n') {
    count++;                //Newline is first char case.
  }
  while (ch != '\n') {
    if (ch == EOF) {
      break;                //No more input case, dont add to count
    } 
    count++;
    ch = fgetc(fp);
    if (ch == '\n') {
      count++;             //If at newLine char case
    }
  }
  fseek(fp, -count, SEEK_CUR);
  return count;
}


void readLine(FILE *fp, char str[]) {   //Needs to be char by char, right now its by string that is wrong bc measureLine goes char by char.
  int len = measureLine(fp);
  char ch;
  for (int i = 0; i < len; i++) {
    ch = fgetc(fp);
    str[i] = ch;
  }
  str[len] = '\0';
  
//  printf("%c\n", str[0]);
}

