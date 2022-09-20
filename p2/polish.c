#include <stdio.h>
#include <stdlib.h>
#include "base.h"
#include "bounds.h"

/*
 *
 */
static long parseExpression() {
    char val = '0';
    char valc = '0';
    long val1 = 0;
    long val2 = 0;
    scanf("%c", &val);
    if (val == '+' || val == '~' || val == '/' || val == '*') {
        scanf("%c", &valc);
        if (valc == '+' || valc == '~' || valc == '/' || valc == '*') {
            
        }
        else {
            ungetc(valc, stdin);
            val1 = readNumber();
            val2 = readNumber();
            if (val == '+') {
                checkAdd(val1, val2);
                return val1 + val2;
            }
            else if (val == '~') {
                checkSub(val1, val2);
                return val1 - val2;
            } 
            else if (val == '*') {
                checkMul(val1,val2);
                return val1 * val2;
            }
            else if (val == '/') {
                checkDiv(val1, val2);
                return val1 / val2;
            }
        }
    }
    else {
        ungetc(val, stdin);
        val1 = readNumber();
        scanf("%c", &val);
        if (val == '+' || val == '~' || val == '/' || val == '*') {
            parseExpression();
        }
        else {
            ungetc(val, stdin);
            val2 = readNumber();
        }
    } 
    return 20;
}

/**
 *
 */
int main() {

   printNumber(parseExpression());

}


