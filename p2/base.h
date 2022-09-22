/**
 * @file base.h
 * @author GIVEN FILE
 * This header file defines the exit statuses and functions that will 
 * be used in both base10 and base32 files as well as polish.c.
*/

/** Exit status indicating that the program was given invalid input. */
#define FAIL_INPUT 102

/**
  Read a number from standard input in the current base, terminating unsuccessfully
  if it's not in the right format or if there's an overflow.
  @return the number read in.
  */ 
long readNumber();

/**
  Print the given value to standard output in the current base.
  @param val the value to print.
  */ 
void printNumber( long val );

