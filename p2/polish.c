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
    else {
        ungetc(c, stdin);
        return readNumber();
    }
}

/**
 *
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
        c = getchar();
    }
    return EXIT_SUCCESS;
}


