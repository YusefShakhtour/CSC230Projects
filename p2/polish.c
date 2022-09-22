/**
 * @file polish.c
 * @author Yusef Shakhtour (yfshakht)
 * This file is responsible for holding our main function and parseExpression
 * which will parse through standard input computing the given operations. 
 * Uses functions from base.h and bounds.h to readNumbers and check for overflow
 */

#include <stdio.h>
#include <stdlib.h>
#include "base.h"
#include "bounds.h"

/*
 * parseExpression is a recursive fucntion that will look through stdin and 
 * parse the polish notation into values and operators that can be used by other 
 * functions in the program. It will then compute the result of the expression
 * @return long the value of the computation
 */
static long parseExpression() {

    long a = 0;
    long b = 0;
    long c = getchar();
    while (c == ' ' || c == '\n') {
        c = getchar();
    }
    if (c == '+') {
        a = parseExpression();
        b = parseExpression();
        checkAdd(a, b);

        return a + b;
    }
    else if (c == '~') {
        a = parseExpression();
        b = parseExpression();
        checkSub(a, b);
        return a - b;
    }
    else if (c == '*') {
        a = parseExpression();
        b = parseExpression();
        checkMul(a, b);

        return a * b;
    }
    else if (c == '/') {
        a = parseExpression();
        b = parseExpression();
        checkDiv(a, b);
        return a / b;
    }
    else { //Otherwise, we have a value, put char back on stream, read the value
        ungetc(c, stdin);
        return readNumber();
    }
}

/**
 * Program starting point, reads characters from stdin 
 * until end of file, skipping white spaces and newlines
 * calls functions as needed
 * @return program exit status
 */
int main() {

    long c = getchar();

    while (c != EOF) {
        while (c == ' ' || c == '\n') {
            c = getchar();
        }
        if (c == EOF) {
            break;
        }
        ungetc(c, stdin);
        c = parseExpression();
        printNumber(c);
        printf("\n"); //After printing, print new line
        c = getchar();
    }
    return EXIT_SUCCESS;
}


