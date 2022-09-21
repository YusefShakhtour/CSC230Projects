/**
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include<limits.h>
#include "bounds.h"

/**
 *
 */
bool isDigit(char ch) {
    if (ch >= '0' && ch <= '9') {
        return true;
    }

    else if (ch >= 'A' && ch <= 'V') {
        return true;
    }
    else {
        return false;
    }
}

/**
 *
 */
long char_to_digit(char ch) {

    if (ch >= '0' && ch <= '9') {
        return (long) (ch - '0');
    }
    else if (ch >= 'A' && ch <= 'V') {
        return (long) (ch - 'A' + 10);
    }
    else {
        exit(EXIT_FAILURE);
    }
}


char digit_to_char(long d) {
    if (d >= 0 && d <= 9) {
        return (char) d;
    }
    else if (d == 10) {
        return 'A';
    }
    else if (d == 11) {
        return 'B';
    }
    else if (d == 12) {
        return 'C';
    }
    else if (d == 13) {
        return 'D';
    }
    else if (d == 14) {
        return 'E';
    }
    else if (d == 15) {
        return 'F';
    }
    else if (d == 16) {
        return 'G';
    }
    else if (d == 17) {
        return 'H';
    }
    else if (d == 18) {
        return 'I';
    }
    else if (d == 19) {
        return 'J';
    }
    else if (d == 20) {
        return 'K';
    }
    else if (d == 21) {
        return 'L';
    }
    else if (d == 22) {
        return 'M';
    }
    else if (d == 23) {
        return 'N';
    }
    else if (d == 24) {
        return 'O';
    }
    else if (d == 25) {
        return 'P';
    }
    else if (d == 26) {
        return 'Q';
    }
    else if (d == 27) {
        return 'R';
    }
    else if (d == 28) {
        return 'S';
    }
    else if (d == 29) {
        return 'T';
    }
    else if (d == 30) {
        return 'U';
    }
    else if (d == 31) {
        return 'V';
    }
    else {
        exit(EXIT_FAILURE);
    }

}

/**
 *
 */
long readNumber() {
    long val = 0;

    char ch = getchar();
    if (ch != '-' && isDigit(ch)) {
        while (isDigit(ch)) { 
            long d = char_to_digit(ch);
            checkMul(val, 32);
            val = val * 32;
            val = val + d;
            ch = getchar();
        }
        ungetc(ch, stdin);
        return val;
    }
    else if (ch == '-') {
        ch = getchar();
        while (isDigit(ch)) {
            long d = char_to_digit(ch);
            checkMul(val, 32);
            val = val * 32;
            val = val + d;
            ch = getchar();
        }
        ungetc(ch, stdin);
        return val * -1;
    }
    else {
        exit(102); // Add invalidInput and all those headers into base.h and incldude it here and in base10.c
    }
}


/*
 *
 */
void printNumber(long val) {
    if (val == 0) {
        printf("%ld\n", val);
    }
    while (val != 0) {
        long d = val % 32;
        if (d >= 0 && d <= 9) { 
            printf("%ld\n", d);
            val = val / 32;
        }
        else if (d >= 10 && d <= 31) { 
            char ch = digit_to_char(d); //Coversion for test 1 goes from the digit 1 to char '\001'
            printf("%c\n", ch);
            val = val / 32;
        }
        else if (d < 0 && d >= -9) {
            printf("%ld\n", d);
            val = val / 32;
        }
        else if (d >= -31 && d <= -10) { 
            char ch = digit_to_char(-1*d); //Coversion for test 1 goes from the digit 1 to char '\001'
            printf("-%c\n", ch);
            val = val / 32;       
        }

    }
}
