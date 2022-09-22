/**
 * @file base10.c
 * @author Yusef Shakhtour (yfshakht)
 * This file will be used in the base10 version of the program where the main function
 * of the file is to read/convert numbers passed in and print those numbers out
 */
#include <stdio.h>
#include <stdlib.h>
#include "base.h"


long readNumber() {
    long val  = 0;
    int valid = scanf("%ld", &val);
    if (valid < 1) { 
        exit(FAIL_INPUT);
    }
    return val;
}


void printNumber(long val) {
    printf("%ld", val);
}
