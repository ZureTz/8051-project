#ifndef STACK_H
#define STACK_H

#include "token.h"

// ==================== Character Stack ====================

#define MAX_CHAR_STACK 16

/**
 * Initialize character stack
 */
void CharStack_Init(void);

/**
 * Check if character stack is empty
 * @return 1 if empty, 0 otherwise
 */
unsigned char CharStack_IsEmpty(void);

/**
 * Check if character stack is full
 * @return 1 if full, 0 otherwise
 */
unsigned char CharStack_IsFull(void);

/**
 * Push a character onto the stack
 * @param ch Character to push
 */
void CharStack_Push(char ch);

/**
 * Pop a character from the stack
 * @return The popped character, or '\0' if stack is empty
 */
char CharStack_Pop(void);

/**
 * Peek at the top character without removing it
 * @return The top character, or '\0' if stack is empty
 */
char CharStack_Peek(void);

// ==================== Float Stack ====================

#define MAX_FLOAT_STACK 8

/**
 * Initialize float stack
 */
void FloatStack_Init(void);

/**
 * Check if float stack is empty
 * @return 1 if empty, 0 otherwise
 */
unsigned char FloatStack_IsEmpty(void);

/**
 * Check if float stack is full
 * @return 1 if full, 0 otherwise
 */
unsigned char FloatStack_IsFull(void);

/**
 * Push a float value onto the stack
 * @param val Float value to push
 */
void FloatStack_Push(float val);

/**
 * Pop a float value from the stack
 * @return The popped value, or 0.0 if stack is empty
 */
float FloatStack_Pop(void);

/**
 * Get the current size of the float stack
 * @return Number of elements in the stack
 */
unsigned char FloatStack_Size(void);

// ==================== Token Queue ====================

#define MAX_TOKEN_QUEUE 32

/**
 * Initialize token queue
 */
void TokenQueue_Init(void);

/**
 * Check if token queue is full
 * @return 1 if full, 0 otherwise
 */
unsigned char TokenQueue_IsFull(void);

/**
 * Add a token to the queue
 * @param token Token to add
 */
void TokenQueue_Add(Token token);

/**
 * Get a token from the queue by index
 * @param index Index of the token
 * @return Pointer to the token
 */
Token *TokenQueue_Get(unsigned char index);

/**
 * Get the current length of the token queue
 * @return Number of tokens in the queue
 */
unsigned char TokenQueue_Length(void);

/**
 * Set the length of the token queue
 * @param len New length
 */
void TokenQueue_SetLength(unsigned char len);

#endif // STACK_H
