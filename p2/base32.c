/**
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include<limits.h>

/**
 *
 */
bool isDigit(char ch) {
    if (ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4' || 
        ch == '5' || ch == '6' || ch == '7' || ch == '8' || ch == '9') {
        return true;
    }
    else if (ch == 'A' || ch == 'B' || ch == 'C' ||
             ch == 'D' || ch == 'E' || ch == 'F' ||
             ch == 'G' || ch == 'H' || ch == 'I' ||
             ch == 'J' || ch == 'K' || ch == 'L' ||
             ch == 'M' || ch == 'N' || ch == 'O' ||
             ch == 'P' || ch == 'Q' || ch == 'R' ||
             ch == 'S' || ch == 'T' || ch == 'U' || 
             ch == 'V') {
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
    long val = 0;
    if (ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4' ||
        ch == '5' || ch == '6' || ch == '7' || ch == '8' || ch == '9') {
        return (long) ch;
    }
    else if (ch == 'A') {
        val = 10;
        return val;
    }
    else if (ch == 'B') {
        val = 11;
        return val;
    }
    else if (ch == 'C') {
        val = 12;
        return val;
    }
    else if (ch == 'D') {
        val = 13;
        return val;
    }
    else if (ch == 'E') {
        val = 14;
        return val;
    }
    else if (ch == 'F') {
        val = 15;
        return val;
    }
    else if (ch == 'G') {
        val = 16;
        return val;
    }
    else if (ch == 'H') {
        val = 17;
        return val;
    }
    else if (ch == 'I') {
        val = 18;
        return val;
    }
    else if (ch == 'J') {
        val = 19;
        return val;
    }
    else if (ch == 'K') {
        val = 20;
        return val;
    }
    else if (ch == 'L') {
        val = 21;
        return val;
    }
    else if (ch == 'M') {
        val = 22;
        return val;
    }
    else if (ch == 'N') {
        val = 23;
        return val;
    }
    else if (ch == 'O') {
        val = 24;
        return val;
    }
    else if (ch == 'P') {
        val = 25;
        return val;
    }
    else if (ch == 'Q') {
        val = 26;
        return val;
    }
    else if (ch == 'R') {
        val = 27;
        return val;
    }
    else if (ch == 'S') {
        val = 28;
        return val;
    }
    else if (ch == 'T') {
        val = 29;
        return val;
    }
    else if (ch == 'U') {
        val = 30;
        return val;
    }
    else if (ch == 'V') {
        val = 31;
        return val;
    }
    else {
        exit(EXIT_FAILURE);
    }
}


char digit_to_char(long d) {
    if (d == 0 || d == 1 || d == 2 || d == 3 || d == 4 ||
        d == 5 || d == 6 || d == 7 || d == 8 || d == 9) {
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

    char ch = scanf("%c", &ch);

    while (isDigit(ch)) { 
        long d = char_to_digit(ch);
        val = val * 32;
        val = val + d;
        ch = scanf("%c", &ch);
    }
    ungetc(ch, stdin);
    return val;
}


/*
 *
 */
void printNumber(long val) {
    while (val != 0) {
        long d = val % 32;
        char ch = digit_to_char(d);
        printf("%c", ch);
        val = val / 32;
    }
}
