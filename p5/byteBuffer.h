/**
 * @file byteBuffer.h
 * @author Yusef Shakhtour (yfshakht)
 * This file prototypes the functions defined in byteBuffer.c that will be used
 * to initialzie and manipulate ByteBuffer structs that hold the data of a file. 
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
 * This method acts as a constructor to create a byteBuffer struct
 * by allocating space for the data and initlializing its length and capacity
 * @return ByteBuffer the initilaized byteBuffer
 */
ByteBuffer *createBuffer();

/**
 * This method adds a particular byte to the data array in a given 
 * ByteBuffer struct changing len and cap as neccesary.
 * @param b the byte to add
 * @param buffer the ByteBuffer to add to
 */
void addByte(ByteBuffer *buffer, byte b);

/**
 * This method frees all allocated memory from a byteBuffer
 * @param buffer the buffer to free
 */
void freeBuffer(ByteBuffer *buffer);

/**
 * This method reads and entire file given the name of the file and stores
 * the entire file in a byteBuffer.
 * @param filename the name of the file to read
 * @return ByteBuffer the byteBuffer that contains the file data
 */
ByteBuffer *readFile(const char *filename);

#endif
