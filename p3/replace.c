/**@file replace.c
 * @author Yusef Shakhtour (yfshakht)
 * This .c file holds main where a majority of error checking will occur and 
 * memory allocation so that other functions will not go out of bounds. 
  */

#include "line.h"
#include "expand.h"
#include <string.h>
#include <stdlib.h>

/** Number of required arguments at the end of the command line. */
#define REQUIRED_ARGS 2


/**
 * The starting point of the program. It error checks and opens and closes necessary files
 * It allocates the necessary amount of memory for various variables that need to be passed 
 * into other functions. As well as retreives necessary command line arguments
 * @param argc the number of elements in argv
 * @param argv the various strings that were passed to the command line
 * @return program exit status
 */
int main(int argc, char *argv[]) { 
  if (argc < REQUIRED_ARGS + 1) { 
    fprintf(stderr, "usage: repalce (<target> <replacement>) * <input-file> <output-file>");
    exit(EXIT_FAILURE);
  }

  FILE *in = fopen(argv[argc - 2], "r");
  FILE *out = fopen(argv[argc - 1], "w");



  if (in == NULL) {
    fprintf(stderr, "Can't open file: %s\n", argv[argc - 2]);
    exit(EXIT_FAILURE);
  }

  if (out == NULL) {
    fprintf(stderr, "Can't open file: %s\n", argv[argc - 1]);
    exit(EXIT_FAILURE);
  }

  int pairs = (argc - 3) / 2;
  char *tList[pairs];
  char *rList[pairs];


  int c1 = 0;
  for (int i = 1; i <= pairs * 2; i = i + 2) { 	//Populating tList with target strings
    tList[c1] = argv[i];
    c1++;
  }

  for (int i = 0; i < pairs; i++) {
    for (int j = i + 1; j < pairs - i; j++) {
      if (strcmp(tList[i], tList[j]) == 0) {
        fprintf(stderr, "Duplicate target: %s\n", tList[i]);
        exit(EXIT_FAILURE);
      }
    }
  }

  int c2 = 0;
  for (int j = 2; j <= pairs * 2; j = j + 2) {              //Populating rList with replacement strings
    rList[c2] = argv[j];
    c2++;
  }

  for (int i = 0; i < pairs; i++) {
    for (int j = 0; j < strlen(rList[i]); j++) { 
      if (!(wordChar(rList[i][j]))) { 
        fprintf(stderr, "Invalid word: %s\n", rList[i]);
        exit(EXIT_FAILURE);
      }
    }
  }

  int maxRep = 0;
  for (int k = 0; k < pairs; k++) {
    if (strlen(rList[k]) > maxRep) {
      maxRep = strlen(rList[k]);
    }
  }                                             //getting maxRep
  
  int destSize = 0;  
  while (measureLine(in) != 0) {
    int lineSize = measureLine(in);
    char str[lineSize + 1];
    str[0] = '\0';
    readLine(in, str);
    destSize = expansionBound(str, maxRep);
    char dest[destSize];
    dest[0] = '\0';
    expand(str, dest, tList, rList, pairs);
    fprintf(out, "%s", dest);

  }
  
  return EXIT_SUCCESS;
}
