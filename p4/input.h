/**@file input.h
 * @author Yusef Shakhtour (yfshakht_
 * This header file prototypes the necessary function for reading in input
 */

/**
 * Will return a whole line in the form of a string from a given file stream
 * @param fp the filestream to read from
 * @return a line from that file
 */
char *readLine(FILE *fp);
