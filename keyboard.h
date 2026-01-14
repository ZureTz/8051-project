#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <reg52.h>

// Matrix Keypad Pin Definitions (Connected to P1)
#define MATRIX_KEYPAD P1

// Independent Keys Pin Definitions (Connected to P3)
sbit KEY_DOT = P3 ^ 0;         // '.'
sbit KEY_LEFT_PAREN = P3 ^ 1;  // '('
sbit KEY_RIGHT_PAREN = P3 ^ 2; // ')'
sbit KEY_BACKSPACE = P3 ^ 3;   // Backspace
sbit KEY_RESERVED_1 = P3 ^ 4;  // Reserved
sbit KEY_RESERVED_2 = P3 ^ 5;  // Reserved
sbit KEY_RESERVED_3 = P3 ^ 6;  // Reserved
sbit KEY_RESERVED_4 = P3 ^ 7;  // Reserved

// Key Code Definitions
// Matrix Keypad (16 keys)
#define KEY_0 '0'
#define KEY_1 '1'
#define KEY_2 '2'
#define KEY_3 '3'
#define KEY_4 '4'
#define KEY_5 '5'
#define KEY_6 '6'
#define KEY_7 '7'
#define KEY_8 '8'
#define KEY_9 '9'
#define KEY_CLEAR 'C'
#define KEY_ADD '+'
#define KEY_SUB '-'
#define KEY_MUL '*'
#define KEY_DIV '/'
#define KEY_EQUAL '='

// Independent Keys (8 keys)
#define KEY_DOT_CHAR '.'
#define KEY_LEFT_PAREN_CHAR '('
#define KEY_RIGHT_PAREN_CHAR ')'
#define KEY_BACKSPACE_CHAR 0x08 // Backspace ASCII code

// No Key Pressed
#define KEY_NONE 0x00

/**
 * @brief Initialize keyboard module
 */
void Keyboard_Init(void);

/**
 * @brief Scan matrix keypad (4x4)
 * @return Key code if pressed, KEY_NONE if no key pressed
 */
unsigned char Keyboard_ScanMatrix(void);

/**
 * @brief Scan independent keys
 * @return Key code if pressed, KEY_NONE if no key pressed
 */
unsigned char Keyboard_ScanIndependent(void);

/**
 * @brief Scan all keyboards (matrix + independent)
 * @return Key code if pressed, KEY_NONE if no key pressed
 */
unsigned char Keyboard_Scan(void);

#endif // KEYBOARD_H