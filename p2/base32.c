/**
 *
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
 * This helper method will convert a given base32 char into its long decimal equivalent
 * @param ch the value in base32
 * @return long the converted value
 */
long char_to_digit(char ch) {

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
char digit_to_char(long d) {
    if (d == 1) {
        return '1';
    }
    else if (d == 2) {
        return '2';
    }
    else if (d == 3) {
        return '3';
    }
    else if (d == 4) {
        return '4';
    }
    else if (d == 5) {
        return '5';
    }
    else if (d == 6) {
        return '6';
    }
    else if (d == 7) {
        return '7';
    }
    else if (d == 8) {
        return '8';
    }
    else if (d == 9) {
        return '9';
    }
    else if (d == 0) {
        return '0';
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
        exit(FAIL_INPUT);
    }

}

/**
 * readNumber if used to read the values in stdin and return said values if they are a digit in base32
 * @return long the value in stdin
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
        exit(FAIL_INPUT); // Add invalidInput and all those headers into base.h and incldude it here and in base10.c
    }
}


/*
 * printNumber is used to print numbers that have been read by readNumber in the correct format
 * @param val the value to print
 */
void printNumber(long val) {

    long d = 0;
    char ch = '0';

    if (val == 0) {
        printf("%c", '0');
    }
    if (val < 0 && val != LONG_MIN) { 
        printf("%c", '-');
        val = val * -1;
    }
    if (val != 0) { //Changed from while to if
        if (val >= 0 && val <= 31) {  //Whole if added
            ch = digit_to_char(val);
            printf("%c", ch);
            return;
        }
        d = val % 32;
        ch = digit_to_char(d);
        printNumber(val / 32); //Added
//        printf("%c", ch);
//        val = val / 32;
        printf("%c", ch); //Added
    }
}
