/**
 *
 */
#include <stdio.h>
#include <stdlib.h>

/***/
#define INVALID_INPUT 102

/**
 * readNumber will read in the value of the digit from stdin and make sure it is valid
 * if it is valid, it will return the read valie
 * @return long the read value from standard input
 */
long readNumber() {
    long val  = 0;
    int valid = scanf("%ld", &val);
    if (valid < 1) { 
        exit(INVALID_INPUT);
    }
    return val;
}

/**
 * printNumber will simply prtint the value of the given parameter
 * in the proper format.
 * @param val the value to print
 */
void printNumber(long val) {
    printf("%ld\n", val);
}
