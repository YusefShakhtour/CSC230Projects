/** 
*/

#ifndef _RIPEMD_H_
#define _RIPEMD_H_

#include "byteBuffer.h"

/** Name for an unsigned 32-bit integer. */
typedef unsigned int longword;

/** Number of bytes in a block. */
#define BLOCK_BYTES 64

/** Number of longwords in a block. */
#define BLOCK_LONGWORDS ( BLOCK_BYTES / sizeof( longword ) )

/** Number of iterations for each round. */
#define RIPE_ITERATIONS 16

/**
 *
 */
void initState(HashState *state);

/**
 *
 */
void padBuffer(ByteBuffer *buffer);

/**
 *
 */
void hashBlock(HashState *state, byte block[BLOCK_BYTES]);

/** Type for a pointer to the bitwise f function used in each round. */
typedef longword (*BitwiseFunction)( longword b, longword c, longword d );

/** Representation for the state of the hash computation.  It's just 4
    unsigned 32-bit integers. Client code can create an instance
    (statically, on the stack or on the heap), but initState() needs
    to initialize it before it can be used. */
typedef struct {
  /** Hash field A used by RipeMD  */
  longword A;
  
  /** Hash field B used by RipeMD  */
  longword B;
  
  /** Hash field C used by RipeMD  */
  longword C;
  
  /** Hash field D used by RipeMD  */
  longword D;
  
  /** Hash field E used by RipeMD  */
  longword E;
  
} HashState;

// If we're compiling for test, expose a collection of wrapper
// functions that let us (indirectly) call internal (static) functions
// in this component.

// You shouldn't need to change the following code, and you don't need
// to worry about commenting it.

#ifdef TESTABLE

longword bitwiseF0Wrapper( longword b, longword c, longword d );
longword bitwiseF1Wrapper( longword b, longword c, longword d );
longword bitwiseF2Wrapper( longword b, longword c, longword d );
longword bitwiseF3Wrapper( longword b, longword c, longword d );
longword bitwiseF4Wrapper( longword b, longword c, longword d );

longword rotateLeftWrapper( longword value, int s );

void hashIterationWrapper( HashState *state,
                           longword datum,
                           int shift,
                           longword noise,
                           BitwiseFunction f );

void hashRoundWrapper( HashState *state,
                       longword data[ BLOCK_LONGWORDS ],
                       int perm[ RIPE_ITERATIONS ],
                       int shift[ RIPE_ITERATIONS ],
                       longword noise,
                       BitwiseFunction f );

#endif

#endif
