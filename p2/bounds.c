/**
 * @file bounds.c
 * @author Yusef Shakhtour (yfshakht)
 * This file is defining the functions that were prototypes in bounds.h
 * The purpose of this file is to determine if certain operations cause overflow or other errors
 * and it will be used in base32.c and base10.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "bounds.h"

/**
 * Absolute is a helper method that will calculate the absolute value of 
 * a given input
 * @param long the value to get the absolute value of
 * @return long the absolute value of that parameter
 */

static long absolute(long a) { 
    if (a < 0) {
        return a * (-1);
    }
    else {
        return a;
    }
}

void checkAdd(long a, long b) {
    if (a > 0 && b > 0) { 
        if (a + b < 0) { 
            exit(OVERFLOW_ERROR);
        }
    }

    else if (a < 0 && b < 0) { 
        if (a + b > 0) {
            exit(OVERFLOW_ERROR);
        }
    }
}


void checkSub(long a, long b) {
    if (a < 0 && b > 0) { 
        if (a - b > 0) {
            exit(OVERFLOW_ERROR);
        }
    }
    else if (a > 0 && b < 0) { 
        if (a - b < 0) {
            exit(OVERFLOW_ERROR);
        }
    }

}


void checkMul(long a, long b) {
    if (a > 0 && b > 0) {
        if (a > (LONG_MAX / b)) {
            exit(OVERFLOW_ERROR);
        }
    }
    else if (a < 0 && b < 0) {
        if (absolute(a) > (LONG_MAX / absolute(b))) {
            exit(OVERFLOW_ERROR);
        }
    }
}

void checkDiv(long a, long b) {
    if (b == 0) {
        exit(DIV_ZERO_ERR);
    }
}
