/**
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

int expansionBound(char *sec, int maxRep) {
  int totalCount; 
  int charCount;
  int pointer = 0;

  while (sec[pointer] != EOF) {
    if (wordChar(sec[pointer])) { 
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

  int maxRep = 0;
  for (int k = 0; k < sizeof(rList) / sizeof(rList[0]); k++) { 
    if (strlen(rList[k]) > maxRep) {
      maxRep = strlen(rList[k]);
    }
  }
  char final[expansionBound(src, maxRep)]; 	 //Use this array to store the new, replaced line and then copy it to dest when done. 
  char temp[100];    				 //"Holding" char array with 100 char buffer that is being used to compare the strings in tList.
  for (int i = 0; i < sizeof(src) / sizeof(char); i++) {
    
    while (wordChar(src[i])) { 
      temp[i] = src[i];
    }
    for (int j = 0; j < pairs; j++) {
      if (strcmp(temp, rList[i]) == 0) { 

        //Should do the replacement here.
        strcpy(temp, rList[i]);
      }
    }

    for (int i = strlen(final); i < strlen(temp); i++) {  //Now whatever is in temp needs to be copied over into destination, in this case to final
      final[i] = temp[i];
    }
  }
}
