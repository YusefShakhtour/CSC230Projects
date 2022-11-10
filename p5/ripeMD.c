/** 
*/

#include "ripeMD.h"
#include "byteBuffer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>



void initState(HashState *state) {
  state->A = 0x67452301;
  state->B = 0xEFCDAB89;
  state->C = 0x98BADCFE;
  state->D = 0x10325476;
  state->E = 0xC3D2E1F0;
}

void padBuffer(ByteBuffer *buffer) {
  int origSize = buffer->len;
  addByte(buffer, 0x80);
  int remainder = 64 - ((origSize + 1) % 64) - 8;
  if (remainder < 0) {  //Case where not enough space for last 8
    remainder += 64;
  }
  for (int i = 0; i < remainder; i++) {
    addByte(buffer, 0x00);                  //Add padding 8 short of multiple of 64
  }
  int bits = origSize * 8;

  for (int i = 0; i < 8; i++) {
    int one = bits % 16;
    bits = bits / 16;
    int two = bits % 16;
    bits = bits / 16;
    addByte(buffer, (two * 16) + one);
  }
}

/**
 * Helper
 */
static longword printHelp(longword letter) {
  longword change = 0x00000000;
  int one = (letter & 0xFF);
  int two = ((letter >> 8) & 0xFF);
  int three = ((letter >> 16) & 0xFF);
  int four = ((letter >> 24) & 0xFF);
  change = (one << 24) | (two << 16) | (three << 8) | (four);
  return change;
}

void printHash(HashState *state) {
  longword newA = printHelp(state->A);
  longword newB = printHelp(state->B);
  longword newC = printHelp(state->C);
  longword newD = printHelp(state->D);
  longword newE = printHelp(state->E);
   
  printf("%08x%08x%08x%08x%08x\n", newA, newB, newC, newD, newE); 
}

static longword bitwiseF0(longword a, longword b, longword c) {
  return (a ^ b ^ c);
}

static longword bitwiseF1 (longword a, longword b, longword c) {
  return ((a & b) | (~a & c));
}

static longword bitwiseF2(longword a, longword b, longword c) {
  return ((a | ~b) ^ c);
}

static longword bitwiseF3(longword a, longword b, longword c) {
  return ((a & c) | (b & ~c));
}

static longword bitwiseF4(longword a, longword b, longword c) {
  return (a ^ (b | ~c));
}

static longword rotateLeft(longword value, int s) {
  return ((value << s) | (value >> (32 - s))); 
}

static void hashIteration(HashState *state, longword datum, int shift, longword noise, BitwiseFunction f) {
  longword newA;
  //Doing all the operations on "newA"
  newA = state->A + f(state->B, state->C, state->D);
  newA += datum;
  newA += noise;
  newA = rotateLeft(newA, shift);
  newA += state->E;

  state->A = state->E;  //Moving e to a
  state->E = state->D;  //Moving d to e
  state->D = rotateLeft(state->C, 10);  //Moving c shifted 10 to d
  state->C = state->B;  //Moving b to c 
  state->B = newA;  //Replacing b with newA
}

static void hashRound(HashState *state, longword data[BLOCK_LONGWORDS], int perm[RIPE_ITERATIONS], int shift[RIPE_ITERATIONS], longword noise, BitwiseFunction f) {
  for (int i = 0; i < 16; i++) {
    hashIteration(state, data[perm[i]], shift[i], noise, f);
  }
}

void hashBlock(HashState *state, byte block[BLOCK_BYTES]) {
  int leftPerm[5][16] = {
  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
  {7, 4, 13, 1, 10, 6, 15, 3, 12, 0, 9, 5, 2, 14, 11, 8},
  {3, 10, 14, 4, 9, 15, 8, 1, 2, 7, 0, 6, 13, 11, 5, 12},
  {1, 9, 11, 10, 0, 8, 12, 4, 13, 3, 7, 15, 14, 5, 6, 2},
  {4, 0, 5, 9, 7, 12, 2, 10, 14, 1, 3, 8, 11, 6, 15, 13}};

  int rightPerm[5][16] = {
  {5, 14, 7, 0, 9, 2, 11, 4, 13, 6, 15, 8, 1, 10, 3, 12},
  {6, 11, 3, 7, 0, 13, 5, 10, 14, 15, 8, 12, 4, 9, 1, 2},
  {15, 5, 1, 3, 7, 14, 6, 9, 11, 8, 12, 2, 10, 0, 4, 13},
  {8, 6, 4, 1, 3, 11, 15, 0, 5, 12, 2, 13, 9, 7, 10, 14},
  {12, 15, 10, 4, 1, 5, 8, 7, 6, 2, 13, 14, 0, 3, 9, 11}};

  int leftShift[5][16] = {
  {11, 14, 15, 12, 5, 8, 7, 9, 11, 13, 14, 15, 6, 7, 9, 8},
  {7, 6, 8, 13, 11, 9, 7, 15, 7, 12, 15, 9, 11, 7, 13, 12},
  {11, 13, 6, 7, 14, 9, 13, 15, 14, 8, 13, 6, 5, 12, 7, 5},
  {11, 12, 14, 15, 14, 15, 9, 8, 9, 14, 5, 6, 8, 6, 5, 12},
  {9, 15, 5, 11, 6, 8, 13, 12, 5, 12, 13, 14, 11, 8, 5, 6}};

  int rightShift[5][16] = {
  {8, 9, 9, 11, 13, 15, 15, 5, 7, 7, 8, 11, 14, 14, 12, 6},
  {9, 13, 15, 7, 12, 8, 9, 11, 7, 7, 12, 7, 6, 15, 13, 11},
  {9, 7, 15, 11, 8, 6, 6, 14, 12, 13, 5, 14, 13, 13, 7, 5},
  {15, 5, 8, 11, 14, 14, 6, 14, 6, 9, 12, 9, 12, 5, 15, 8},
  {8, 5, 12, 9, 12, 5, 14, 6, 8, 13, 6, 5, 15, 13, 11, 11}};

  longword leftNoise[5] = {0x00000000, 0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xA953FD4E};
  longword rightNoise[5] = {0x50A28BE6, 0x5C4DD124, 0x6D703EF3, 0x7A6D76E9, 0x00000000};

  longword (*leftBitwise[5]) (longword a, longword b, longword c) = {bitwiseF0, bitwiseF1, bitwiseF2, bitwiseF3, bitwiseF4};
  longword (*rightBitwise[5]) (longword a, longword b, longword c) = {bitwiseF4, bitwiseF3, bitwiseF2, bitwiseF1, bitwiseF0};
  
  longword data[16];
  for (int i = 0; i < 16; i++) {
    int one = block[4 * i + 3];
    int two = block[4 * i + 2];
    int three = block[4 * i + 1];
    int four = block[4 * i + 0];
    longword lw = 0x0;
    lw = (one << 24) | (two << 16) | (three << 8) | (four);
    data[i] = lw;
  }

  HashState leftState;
  HashState rightState;
  initState(&leftState);
  initState(&rightState);
  leftState = *state;
  rightState = *state;

 
  //Left Side rounds
  for (int i = 0; i < 5; i++) {
    hashRound(&leftState, data, *(leftPerm + i), *(leftShift + i), leftNoise[i], leftBitwise[i]);
  } 
  //Right side rounds
  for (int i = 0; i < 5; i++) {
    hashRound(&rightState, data, *(rightPerm + i), *(rightShift + i), rightNoise[i], rightBitwise[i]);
  }
 
  //Doing all the operations on "newA"
  //  newA = state->A + f(state->B, state->C, state->D);
  //  newA += datum;
  //  newA += noise;
  //  newA = rotateLeft(newA, shift);
  //  newA += state->E;
  //
  //  state->A = state->E;  //Moving e to a
  //  state->E = state->D;  //Moving d to e
  //  state->D = rotateLeft(state->C, 10);  //Moving c shifted 10 to d
  //  state->C = state->B;  //Moving b to c
  //  state->B = newA;  //Replacing b with newA

  longword newA = state->B + leftState.C + rightState.D;
  longword newB = state->C + leftState.D + rightState.E;
  longword newC = state->D + leftState.E + rightState.A;
  longword newD = state->E + leftState.A + rightState.B;
  longword newE = state->A + leftState.B + rightState.C;
  
  state->A = newA;
  state->B = newB;
  state->C = newC;
  state->D = newD;
  state->E = newE;
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
