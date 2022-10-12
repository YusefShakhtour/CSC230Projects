/**
  */

#include "line.h"
#include "expand.h"
#include <string.h>

/** Number of required arguments at the end of the command line. */
#define REQUIRED_ARGS 2


/**
 *
 */
int main(int argc, char *argv[]) { 
  if (argc < REQUIRED_ARGS + 1) { 
    //print to stdError
  }
  else if ((argc % 2) != 1) {
    //print to stdError
  }

  FILE *in = fopen(argv[argc - 2], "r");
  FILE *out = fopen(argv[argc - 1], "w");

  int l = measureLine(in);

  if (!in || !out) {
    //print to stdError
  }

  int pairs = (argc - 3) / 2;
  char *tList[pairs];
  char *rList[pairs];
  if (argc > 3) { 
    pairs = (argc - 3) / 2;
  }

  int c1 = 0;
  for (int i = 1; i <= pairs * 2; i = i + 2) { 	//Populating tList with target strings
     // Just point to start of each string from argv
    tList[c1] = argv[i];
    c1++;
  }

  int c2 = 0;
  for (int j = 2; j <= pairs * 2; j = j + 2) {              //Populating rList with replacement strings
//      strcpy(rList[c2], argv[j]);
    rList[c2] = argv[j];
    c2++;
  }

  int maxRep = 0;
  for (int k = 0; k < pairs; k++) {
    if (strlen(rList[k]) > maxRep) {
      maxRep = strlen(rList[k]);
    }
  }                                             //getting maxRep
  
  
//  int c = 0;
  int destSize = 0;  
  while (measureLine(in) != 0) {
    int lineSize = measureLine(in);
    printf("%d\n", lineSize);
    char str[lineSize + 1];
    str[0] = '\0';
    readLine(in, str);
    destSize = expansionBound(str, maxRep);
    char dest[destSize];
    dest[0] = '\0';
    expand(str, dest, tList, rList, pairs);
    fprintf(out, "%s", dest);
//    c = c + 1;
  }
  



}
