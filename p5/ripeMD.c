/** 
*/

#include "ripeMD.h"


void initState(HashState *state) {

}

void padBuffer(ByteBuffer *buffer) {

}

void printHash(HashState *state) {

}

static longword bitwiseF0(longword a, longword b, longword c) {

}

static longword bitwiseF1 (longword a, longword b, longword c) {

}

static longword bitwiseF2(longword a, longword b, longword c) {

}

static longword bitwiseF3(longword a, longword b, longword c) {

}

static longword bitwiseF4(longword a, longword b, longword c) {

}

static longword rotateLeft(longword value, int s) {

}

static void hashIteration(HashState *state, longword datum, int shift, longword noise, BitwiseFunction f) {

}

static void hashRound(HashState *state, longword data[BLOCK_LONGWORDS], int perm[RIPE_ITERATIONS], int shift[RIPE_ITERATIONS], longword noise, BitwiseFunction f) {

}

void hashBlock(HashState *state, byte block[BLOCK_BYTES]) {

}


// Put the following at the end of your implementation file.
// If we're compiling for unit tests, create wrappers for the otherwise
// private functions we'd like to be able to test.

#ifdef TESTABLE

longword bitwiseF0Wrapper( longword b, longword c, longword d )
{
  return bitwiseF0( b, c, d );
}

longword bitwiseF1Wrapper( longword b, longword c, longword d )
{
  return bitwiseF1( b, c, d );
}

longword bitwiseF2Wrapper( longword b, longword c, longword d )
{
  return bitwiseF2( b, c, d );
}

longword bitwiseF3Wrapper( longword b, longword c, longword d )
{
  return bitwiseF3( b, c, d );
}

longword bitwiseF4Wrapper( longword b, longword c, longword d )
{
  return bitwiseF4( b, c, d );
}

longword rotateLeftWrapper( longword value, int s )
{
  return rotateLeft( value, s );
}

void hashIterationWrapper( HashState *state,
                           longword datum,
                           int shift,
                           longword noise,
                           BitwiseFunction f )
{
  hashIteration( state, datum, shift, noise, f );
}

void hashRoundWrapper( HashState *state,
                       longword *block,
                       int perm[ RIPE_ITERATIONS ],
                       int shift[ RIPE_ITERATIONS ],
                       longword noise,
                       BitwiseFunction f )
{
  hashRound( state, block, perm, shift, noise, f );
}

#endif
