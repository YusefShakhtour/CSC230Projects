/**
  */

#include "ripeMD.h"
#include "byteBuffer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
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
  for (int i = 0; i < (b->len) / 64; i++) {
    hashBlock(&state, &(b->data)[i * 64]);
  } 
  printHash(&state); 

  freeBuffer(b); 
  return EXIT_SUCCESS;
}
