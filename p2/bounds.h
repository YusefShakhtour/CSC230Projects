/**
 * @file bounds.h
 * @author Yusef Shakhtour (yfshakht)
 * This header file defines the exit statuses that will be used for errors
 * in bounds.c
 * It will also prototype the functions used in bounds.c which will determine
 * whether or not a given operation will overflow
 */

/** Error code that will be used if overflow is detected */
#define OVERFLOW_ERROR 100

/** Error code that will be used if a divide by 0 error occurs */
#define DIV_ZERO_ERR 101

/**
 * checkAdd will ensure that a given addition computation will not cause overflow
 * @param a one of the operands of the expression
 * @param b one of the operands of the expression
 */
void checkAdd(long a, long b);

/**
 * checkSub will ensure that a given subtraction computation will not cause overflow
 * @param a one of the operands of the expression
 * @param b one of the operands of the expression
 */
void checkSub(long a, long b);

/**
 * checkMul will ensure that a given multiplication computation will not cause overflow
 * @param a one of the operands of the expression
 * @param b one of the operands of the expression
 */
void checkMul(long a, long b);

/**
 * checkDiv will ensure that a given division computation will not cause a div by 0 error
 * @param a one of the operands of the expression
 * @param b one of the operands of the expression
 */
void checkDiv(long a, long b);


