/**@file expand.c
 * @author Yusef Shakhtour (yfshakht)
 * This .c file defines the functions that will be used to replace given target strings with replacement strings
 * and store the new string in a separate variable to be used later
  */
#include <string.h>
#include "expand.h"
#include <stdio.h>

bool wordChar(int ch) {
  if ((ch >= 'A' && ch <= 'Z') ||
      (ch >= '0' && ch <= '9') ||
      (ch >= 'a' && ch <= 'z') || 
      (ch == '_')) { 
    return true;
  }
  else {
    return false;
  }

}

int expansionBound(char *src, int maxRep) {
  int totalCount = 0; 
  int charCount = 0;
  int pointer = 0;

  while (src[pointer] != '\0') {
    if (wordChar(src[pointer])) { 
      totalCount++;
      charCount++;
      pointer++;
    }
    else {
        totalCount++;
      if (charCount != 0 && charCount <= maxRep) { 
        totalCount += maxRep - charCount;
      }
        charCount = 0;
        pointer++;
    }
  }
  return totalCount; 
}

void expand(char *src, char *dest, char *tList[], char *rList[], int pairs) {
  dest[0] = '\0';
  int maxRep = 0;
  for (int k = 0; k < pairs; k++) {
    if (strlen(rList[k]) > maxRep) { //Need largest possible value for temp
      maxRep = strlen(rList[k]);
    }
  }

  char temp[expansionBound(src, maxRep)];
  temp[0] = '\0';

  for (int i = 0; i < strlen(src); i++) {
    if (wordChar(src[i])) {
      int count1 = 0;
      while (wordChar(src[i])) {
        temp[count1] = src[i];
        i++;
        count1++;
      }
      temp[count1] = '\0';
      i--;
      

      for (int k = 0; k < pairs; k++) {
        if (strcmp(tList[k], temp) == 0) {
          strcpy(temp, rList[k]);
        }
      }

      int len = strlen(dest);
      int count = 0;
      for (int j = len; j <= len + strlen(temp); j++) {
        dest[j] = temp[count];
        count++;
      }
    }
    
    else {
      int count2 = 0; 
      while (!(wordChar(src[i]))) {
        if (src[i] == '\0') {
          break;
        }
        temp[count2] = src[i];
        i++;
        count2++;
      }
      temp[count2] = '\0';
      i--;

      int len1 = strlen(dest);
      int count4 = 0;
      for (int j = len1; j < len1 + strlen(temp); j++) {
        dest[j] = temp[count4];
        if (temp[count4] == '\0') {
          break;
        }
        if (temp[count4] == '\n') {
          dest[j + 1] = '\0';
          goto end;
        }
        count4++;
      }
    dest[len1 + strlen(temp)] = '\0';
    }
  }
  end:;
}
