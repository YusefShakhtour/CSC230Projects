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
  char temp[sizeof(dest)];
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
      for (int j = len; j <= len + strlen(temp); j++) {  //Problem with handling non-word chars. Always gives garbage, might be going out of bounds.
        dest[j] = temp[count];
        count++;
      }
    }
    
    else {                                              //ISSUE**** If we have a non-word char followed by \n, how to handle?
      int count2 = 0;                                   //ISSUE**** If we have a null-terminator, this needs to be handled to exit the loop 
      while (!(wordChar(src[i]))) {
        if (src[i] == '\0') {                       //HAVE TO STOP AT NULL TERMINATOR THINK OF IT AS END OF SRC CASE
    //      count2++;
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

//int main() {
//  char src[] = "Testing this string to see if it works";
//  int size = expansionBound(src, 4);
//  printf("%d\n", size);
//  char dest[size];
////  char dest[45];                         //Hard coding the 45 into dest allows expand to work perfectly, However, when I leave it to the variable size, 
//                                           //which is still the correct integer value, for whatever reason there are discrepencies in my destination string  
//  char *tList[] = {"this", "see"};
//  char *rList[] = {"with" , "be"};
//
//  expand(src, dest, tList, rList, 2); 
//  FILE *t = fopen("myFile.txt", "w");
//  fprintf(t, "%s", dest);
//  return 0;
//  
//}
