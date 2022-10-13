/**@file expand.h
 * @author Yusef Shakhtour (yfshakht)
 * This header file prototypes the functions used and defined in expand.c that will be used
 * in replace.c's main method
  */

#include <stdbool.h>

/**
 * This function will check if a given character input is considered a word character
 * @param ch the character to check
 * @return bool whether or not the character is a word character
 */
bool wordChar(int ch);

/**
 * This function will take a string and integer input and return the longest possible string
 * that can be made if every character less than maxRep was replaced with maxRep.
 * @param src the string to get the maximum for
 * @param maxRep the length of the largest replacement string available
 * @return the lenght of the longest possible string
 */
int expansionBound(char *src, int maxRep);

/**
 * This function will replace target words in a string with replacement words and store the newly created string into a separate string.
 * @param src the original string
 * @param dest the string with replacements
 * @param tList a list of pointers to target strings
 * @param rList a list of pointers to replacement strings
 * @param pairs the lenght of rList and tList
 */
void expand(char *src, char *dest, char *tList[], char *rList[], int pairs);
