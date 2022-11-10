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

//
void addByte(ByteBuffer *buffer, byte b) {
  if (buffer->cap > buffer->len) {  
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

  free(buffer->data);
  free(buffer);
}


ByteBuffer *readFile(const char *filename) { 
  ByteBuffer *buffer = createBuffer();
  FILE *fp = fopen(filename, "rb");
  if (fp == NULL) {
    return NULL;
  } 
                   
  fseek(fp, 0, SEEK_END);   //Go to end of file
  long end = ftell(fp);     //Tell me where that end of file is
  fseek(fp, 0, SEEK_SET);   //Go back to beginning
  char ch = fgetc(fp);      
  int count = 0;           
  while (count < end) {   //Keep reading until we reach the final position in the file
    addByte(buffer, ch);
    ch = fgetc(fp);
    count++;
  } 
  fclose(fp); 
  return buffer; 
}
