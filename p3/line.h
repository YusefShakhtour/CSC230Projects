/**@file line.h
 * @author Yusef Shakhtour (yfshakht)
 * This is a header file that prototypes necessary fucntions to be used in replace.c
  */

#include <stdio.h>

/**
 * Given a file, get the lenght of a single line from the file
 * @param fp the file to be passed in
 * @return int the length of the line, or 0 if there are no more lines
 */
int measureLine(FILE *fp);

/**
 * Given a file and a string, read the next line from the file and store it into the string
 * @param fp the file to read from
 * @param str the string to store the line into
 */
void readLine(FILE *fp, char str[]);
