/**
 * @file base32.c
 * @author Yusef Shakhtour (yfshakht)
 * This file will be used in the base32 version of the program where several helper
 * methods and the printNumber and readNumber methods are used to read, convert, then print
 * values from decimal to base32 and vice versa when neccessary.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include<limits.h>
#include "bounds.h"
#include "base.h"

/**
 * This helper method will determine if the given char is a valid digit in base32
 * @param ch the character to check validity
 * @return bool whether or not the character is valid
 */
static bool isDigit(char ch) {
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
 * This helper method will convert a given base32 char into its long decimal equivalent
 * @param ch the value in base32
 * @return long the converted value
 */
static long char_to_digit(char ch) {

    if (ch >= '0' && ch <= '9') {
        return (long) (ch - '0');
    }
    else if (ch >= 'A' && ch <= 'V') {
        return (long) (ch - 'A' + 10);
    }
    else {
        exit(FAIL_INPUT);
    }
}

/**
 * This helper method will convert a given long input into it's equivalent base32 character
 * @param d the long value to convert
 * @return char the long value in base32
 */
static char digit_to_char(long d) {
    if (d >= 0 && d <= 9) {
        return (char) (d + 48);
    }
    else if (d >= 10 && d <= 31) {
        return (char) (d + 55);
    }
    else {
        exit(FAIL_INPUT);
    }

}


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
        exit(FAIL_INPUT);
    }
}


void printNumber(long val) {

    long d = 0;
    char ch = '0';

    if (val == 0) {
        printf("%c", '0');
    }
    if (val < 0 && val != LONG_MIN) {  //Special case for if value is Long_Min
        printf("%c", '-');
        val = val * -1;
    }
    if (val != 0) {
        if (val >= 0 && val <= 31) {  //If on last digit
            ch = digit_to_char(val);
            printf("%c", ch);
            return;
        }
        d = val % 32;
        ch = digit_to_char(d);
        printNumber(val / 32);
        printf("%c", ch);
    }
}
