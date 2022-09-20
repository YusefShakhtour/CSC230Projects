/**
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/**
 *
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
 *
 */
void checkAdd(long a, long b) {
    if (a > 0 && b > 0) { 
        if (a + b < 0) { 
            exit(EXIT_FAILURE);
        }
    }

    else if (a < 0 && b < 0) { 
        if (a + b > 0) {
            exit(EXIT_FAILURE);
        }
    }
    else if (a > 0 && b < 0) {
        if (absolute(b) > a) {
            if (a + b > 0) { 
                exit(EXIT_FAILURE);
            }
        }
        else if (absolute(b) < a) {
            if (a + b < 0) {
                exit(EXIT_FAILURE);
            }
        }
    }
    else if (a < 0 && b > 0) {
        if (absolute(a) > b) {
            if (a + b > 0) {
                exit(EXIT_FAILURE);
            }
        }
        else if (absolute(a) < b) {
            if (a + b < 0) {
                exit(EXIT_FAILURE);
            }
        }
    }
}

/**
 *
 */
void checkSub(long a, long b) {
    if (a < 0 && b > 0) { 
        if (a - b > 0) {
            exit(EXIT_FAILURE);
        }
    }
    else if (a > 0 && b < 0) { 
        if (a - b < 0) {
            exit(EXIT_FAILURE);
        }
    }

}

/**
 *
 */
void checkMul(long a, long b) {
    if (a > 0 && b > 0) {
        if (a > (LONG_MAX / b)) {
            exit(EXIT_FAILURE);
        }
    }
    else if (a < 0 && b < 0) {
        if (absolute(a) > (LONG_MAX / absolute(b))) {
            exit(EXIT_FAILURE);
        }
    }
}

/**
 *
 */
void checkDiv(long a, long b) {


}
