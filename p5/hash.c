/**@file hash.c
 * @author Yusef Shakhtour (yfshakht)
 * This file holds the main function and uses various fucntions
 * defined in byteBuffer.h and ripeMD.h to perform the RIPEMD algorithm
 * on a file that is passed via the commandline and print the hashed value
 */

#include "ripeMD.h"
#include "byteBuffer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/** Number of arguments that should be present in commandline */
#define REQUIRED_ARGS 2


/**
 * Programs starting point that takes a filename given on the commandline and performs 
 * the RIPEMD hash algorithm on that file to eventually print out the hashed file.
 * @param argc number of command line arguments
 * @param argv list of command line arguments
 * @return program exit status
 */
int main(int argc, char *argv[]) {
  if (argc != REQUIRED_ARGS) {
    fprintf(stderr, "usage: hash <input-file>\n");
    exit(EXIT_FAILURE);
  }
  ByteBuffer *b = readFile(argv[1]);
  if (b == NULL) {
    perror(argv[1]);
    exit(EXIT_FAILURE);
  }
  padBuffer(b);
  HashState state;
  initState(&state);
  for (int i = 0; i < (b->len) / BLOCK_BYTES; i++) {
    hashBlock(&state, &(b->data)[i * BLOCK_BYTES]);
  } 
  printHash(&state); 

  freeBuffer(b); 
  return EXIT_SUCCESS;
}
