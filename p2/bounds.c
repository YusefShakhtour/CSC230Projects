/**
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define OVERFLOW_ERROR 100
#define DIV_ZERO_ERR 101

/**
 * Absolute is a helper method that will calculate the absolute value of 
 * a given input
 * @param long the value to get the absolute value of
 * @return long the absolute value of that parameter
 */

long absolute(long a) { 
    if (a < 0) {
        return a * (-1);
    }
    else {
        return a;
    }
}

/**
 * checkAdd will ensure that a given addition computation will not cause overflow
 * @param a one of the operands of the expression
 * @param b one of the operands of the expression
 */
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

/**
 * checkSub will ensure that a given subtraction computation will not cause overflow
 * @param a one of the operands of the expression
 * @param b one of the operands of the expression
 */
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

/**
 * checkMul will ensure that a given multiplication computation will not cause overflow
 * @param a one of the operands of the expression
 * @param b one of the operands of the expression
 */
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

/**
 * checkDiv will ensure that the given division operation will not cause a divide by 0 error
 * @param a one of the operands of the expression
 * @param b one of the operands of the expression
 */
void checkDiv(long a, long b) {
    if (b == 0) {
        exit(DIV_ZERO_ERR);
    }
}
