#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "token.h"

// Maximum expression length (32 characters with scrolling display)
#define MAX_EXPR_LEN 32

// LCD display window size
#define LCD_DISPLAY_WIDTH 16

// Error codes
#define CALC_OK 0
#define CALC_ERR_SYNTAX 1
#define CALC_ERR_DIV_ZERO 2
#define CALC_ERR_OVERFLOW 3

// Calculator main interface functions

/**
 * Initialize calculator
 */
void Calculator_Init(void);

/**
 * Add a character to the expression
 * @param ch Input character
 * @return 1=success, 0=failure (buffer full or invalid character)
 */
unsigned char Calculator_InputChar(char ch);

/**
 * Delete the last character in the expression (backspace)
 */
void Calculator_Backspace(void);

/**
 * Clear the expression
 */
void Calculator_Clear(void);

/**
 * Get the current expression string
 * @return Pointer to the expression string
 */
char *Calculator_GetExpression(void);

/**
 * Get a window of the expression for LCD display (16 characters)
 * @param buffer Buffer to store the display window (at least 17 bytes)
 * @param offset Offset position in the expression (0 to MAX_EXPR_LEN-LCD_DISPLAY_WIDTH)
 */
void Calculator_GetDisplayWindow(char *buffer, unsigned char offset);

/**
 * Get the maximum scroll offset for current expression
 * @return Maximum offset (0 if expression <= 16 chars)
 */
unsigned char Calculator_GetMaxScrollOffset(void);

/**
 * Evaluate the expression result
 * @param result Result string buffer (at least 17 bytes, including \0)
 * @return Error code (CALC_OK, CALC_ERR_SYNTAX, etc.)
 */
unsigned char Calculator_Evaluate(char *result);

#endif // CALCULATOR_H