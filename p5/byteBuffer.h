/** 
*/

#ifndef _BYTE_BUFFER_H_
#define _BYTE_BUFFER_H_

/** Number of bits in a byte */
#define BBITS 8

/** Type used as a byte. */
typedef unsigned char byte;

/** Representation for a file copied to memory, with some padding
    at the end. */
typedef struct {
  /** Array of bytes from the file (no null termination, so it's not a
      string). */
  byte *data;

  /** Number of currently used bytes in the data array. */
  unsigned int len;

  /** Capacity of the data array (it's typically over-allocated. */
  unsigned int cap;
} ByteBuffer;

/**
 *
 */
ByteBuffer *createBuffer();

/**
 *
 */
void addByte(ByteBuffer *buffer, byte b);

/**
 *
 */
void freeBuffer(ByteBuffer *buffer);

/**
 *
 */
ByteBuffer *readFile(const char *filename);

#endif
