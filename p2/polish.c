#include <stdio.h>
#include <stdlib.h>
#include "base.h"
#include "bounds.h"

/*
 *
 */
static long parseExpression() {

    long a = 0;
    long b = 0;
    long c = getchar();
    while (c == ' ') {
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
            printf("\n"); 
            break;
        }

        ungetc(c, stdin);
        c = parseExpression();
        printNumber(c);
        c = getchar();
    }
    return EXIT_SUCCESS;
}


