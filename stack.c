#include <reg52.h>

#include "utils.h"
#include "stack.h"

// ==================== Character Stack Implementation ====================

static char xdata charStack[MAX_CHAR_STACK];
static unsigned char charStackTop = 0;

void CharStack_Init(void)
{
  charStackTop = 0;
}

unsigned char CharStack_IsEmpty(void)
{
  return charStackTop == 0;
}

unsigned char CharStack_IsFull(void)
{
  return charStackTop >= MAX_CHAR_STACK;
}

void CharStack_Push(char ch)
{
  if (!CharStack_IsFull())
  {
    charStack[charStackTop++] = ch;
  }
}

char CharStack_Pop(void)
{
  if (!CharStack_IsEmpty())
  {
    return charStack[--charStackTop];
  }
  return '\0';
}

char CharStack_Peek(void)
{
  if (!CharStack_IsEmpty())
  {
    return charStack[charStackTop - 1];
  }
  return '\0';
}

// ==================== Float Stack Implementation ====================

static float xdata floatStack[MAX_FLOAT_STACK];
static unsigned char floatStackTop = 0;

void FloatStack_Init(void)
{
  floatStackTop = 0;
}

unsigned char FloatStack_IsEmpty(void)
{
  return floatStackTop == 0;
}

unsigned char FloatStack_IsFull(void)
{
  return floatStackTop >= MAX_FLOAT_STACK;
}

void FloatStack_Push(float val)
{
  if (!FloatStack_IsFull())
  {
    floatStack[floatStackTop++] = val;
  }
}

float FloatStack_Pop(void)
{
  if (!FloatStack_IsEmpty())
  {
    return floatStack[--floatStackTop];
  }
  return 0.0;
}

unsigned char FloatStack_Size(void)
{
  return floatStackTop;
}

// ==================== Token Queue Implementation ====================

static Token xdata tokenQueue[MAX_TOKEN_QUEUE];
static unsigned char tokenQueueLen = 0;

void TokenQueue_Init(void)
{
  tokenQueueLen = 0;
}

unsigned char TokenQueue_IsFull(void)
{
  return tokenQueueLen >= MAX_TOKEN_QUEUE;
}

void TokenQueue_Add(Token token)
{
  if (!TokenQueue_IsFull())
  {
    tokenQueue[tokenQueueLen++] = token;
  }
}

Token *TokenQueue_Get(unsigned char index)
{
  if (index < tokenQueueLen)
  {
    return &tokenQueue[index];
  }
  return NULL;
}

unsigned char TokenQueue_Length(void)
{
  return tokenQueueLen;
}

void TokenQueue_SetLength(unsigned char len)
{
  if (len <= MAX_TOKEN_QUEUE)
  {
    tokenQueueLen = len;
  }
}
