/** 
*/

#include "byteBuffer.h"
#include <stdio.h>
#include <stdlib.h>

ByteBuffer *createBuffer() {
  ByteBuffer *buffer = (ByteBuffer *)malloc(sizeof(ByteBuffer));  //Changed to pointer
  buffer->len = 0;
  buffer->cap = 5;
  buffer->data = (byte *)malloc(buffer->cap * sizeof(byte));
  return buffer;
}


void addByte(ByteBuffer *buffer, byte b) {
  if (buffer->cap > buffer->len) {      //Need to allocate data for every add? Or did the allocation in create already allocate enough for all of capacity?
    buffer->data[buffer->len] = b;
    buffer->len += 1;
  }
  else {
    buffer->cap *= 2;
    buffer->data = (byte *)realloc(buffer->data, buffer->cap * sizeof(byte));
    buffer->data[buffer->len] = b;
    buffer->len += 1;
  }
}


void freeBuffer(ByteBuffer *buffer) {
//  for (int i = 0; i < buffer->cap; i++) {
//    free(buffer->list[i]);
//  }
  free(buffer->data);
  free(buffer);
}


ByteBuffer *readFile(const char *filename) { 
  ByteBuffer *buffer = createBuffer();
  FILE *fp = fopen(filename, "rb");
  if (fp == NULL) {
    return NULL;
  }                      //If wanting to go char at a time use feof
                         //char is signed, need to be unsigned. Might cast but might not.  
  char ch = fgetc(fp);   //Use fread fseek and ftell to read the whole thing at once
  while (ch != EOF) {   //Read everything into data
    addByte(buffer, ch);
    ch = fgetc(fp);
  }
  return buffer; 
}
