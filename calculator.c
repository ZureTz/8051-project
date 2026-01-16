#include <reg52.h>
#include <string.h>
#include <stdio.h>

#include "calculator.h"
#include "stack.h"

// ==================== Global Variables ====================

// Expression buffer (user input)
static char xdata expressionBuffer[MAX_EXPR_LEN + 1];
static unsigned char expressionLen = 0;

// Operator precedence table (stored in code memory)
// Initialized at runtime to avoid C51 designated initializer syntax issues
static unsigned char xdata operatorPrecedence[128];

/**
 * Initialize operator precedence table
 */
static void InitOperatorPrecedence(void)
{
  unsigned char i;
  // Clear all entries
  for (i = 0; i < 128; i++)
  {
    operatorPrecedence[i] = 0;
  }
  // Set operator precedences
  operatorPrecedence['('] = 0;
  operatorPrecedence[')'] = 0;
  operatorPrecedence['+'] = 1;
  operatorPrecedence['-'] = 1;
  operatorPrecedence['*'] = 2;
  operatorPrecedence['/'] = 2;
}

// ==================== Helper Functions ====================

/**
 * Check if character is a digit or decimal point
 */
static unsigned char IsDigitOrDot(char ch)
{
  return (ch >= '0' && ch <= '9') || ch == '.';
}

/**
 * Check if character is an operator
 */
static unsigned char IsOperator(char ch)
{
  return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

/**
 * Get operator precedence
 */
static unsigned char GetPrecedence(char op)
{
  return operatorPrecedence[(unsigned char)op];
}

/**
 * Perform binary operation
 * @return CALC_OK or error code
 */
static unsigned char PerformOperation(char op, float operand1, float operand2, float *result)
{
  switch (op)
  {
  case '+':
    *result = operand1 + operand2;
    return CALC_OK;
  case '-':
    *result = operand1 - operand2;
    return CALC_OK;
  case '*':
    *result = operand1 * operand2;
    return CALC_OK;
  case '/':
    if (operand2 == 0.0 || operand2 == -0.0)
    {
      return CALC_ERR_DIV_ZERO;
    }
    *result = operand1 / operand2;
    return CALC_OK;
  default:
    return CALC_ERR_SYNTAX;
  }
}

/**
 * Convert string to float (manual implementation to avoid library dependencies)
 */
static float StringToFloat(const char *str, unsigned char len)
{
  float result = 0.0;
  float decimal = 0.0;
  unsigned char i = 0;
  unsigned char isNegative = 0;
  unsigned char afterDot = 0;
  float divisor = 10.0;

  // Negative sign
  if (str[0] == '-')
  {
    isNegative = 1;
    i = 1;
  }

  for (; i < len; i++)
  {
    if (str[i] == '.')
    {
      afterDot = 1;
      continue;
    }

    // Is number
    if (str[i] >= '0' && str[i] <= '9')
    {
      if (!afterDot)
      {
        result = result * 10.0 + (str[i] - '0');
      }
      else
      {
        decimal = decimal + (str[i] - '0') / divisor;
        divisor *= 10.0;
      }
    }
  }

  result += decimal;
  if (isNegative)
  {
    result = -result;
  }
  return result;
}

/**
 * Convert float to string (5 decimal places)
 */
static void FloatToString(float value, char *buffer)
{
  long intPart;
  long fracPart;
  unsigned char i;
  unsigned char negative = 0;

  // Handle negative numbers
  if (value < 0)
  {
    negative = 1;
    value = -value;
  }

  // Round to 5 decimal places
  value += 0.000005;

  // Extract integer and fractional parts
  intPart = (long)value;
  fracPart = (long)((value - intPart) * 100000);

  // Format output using sprintf
  if (negative)
  {
    sprintf(buffer, "-%ld.%05ld", intPart, fracPart);
  }
  else
  {
    sprintf(buffer, "%ld.%05ld", intPart, fracPart);
  }

  // Remove trailing zeros (keep at least 1 decimal place)
  i = strlen(buffer) - 1;
  while (i > 0 && buffer[i] == '0' && buffer[i - 1] != '.')
  {
    buffer[i] = '\0';
    i--;
  }
}

// ==================== Lexical Analysis ====================

/**
 * Tokenize: Convert expression string to token sequence
 * @return CALC_OK or error code
 */
static unsigned char Tokenize(void)
{
  unsigned char i = 0;
  char xdata numBuffer[16];
  unsigned char numLen = 0;
  unsigned char hasDot = 0;
  Token token;
  unsigned char lastTokenType = TOKEN_OPERATOR; // Assume operator at start

  TokenQueue_Init();

  while (i < expressionLen)
  {
    char ch = expressionBuffer[i];

    // Skip whitespace
    if (ch == ' ')
    {
      i++;
      continue;
    }

    // Process numbers (including negative numbers)
    // Negative sign is treated as part of number if:
    // - At the beginning of expression
    // - After an operator
    // - After a left parenthesis
    if ((ch >= '0' && ch <= '9') || ch == '.' ||
        (ch == '-' && (lastTokenType == TOKEN_OPERATOR || lastTokenType == TOKEN_LPAREN)))
    {
      numLen = 0;
      hasDot = 0;

      // Include negative sign in number
      if (ch == '-')
      {
        numBuffer[numLen++] = expressionBuffer[i++];
      }

      while (i < expressionLen && IsDigitOrDot(expressionBuffer[i]))
      {
        if (expressionBuffer[i] == '.')
        {
          if (hasDot)
          {
            return CALC_ERR_SYNTAX; // Multiple decimal points
          }
          hasDot = 1;
        }
        numBuffer[numLen++] = expressionBuffer[i++];
      }

      numBuffer[numLen] = '\0';
      token.type = TOKEN_NUMBER;
      token.value = StringToFloat(numBuffer, numLen);
      TokenQueue_Add(token);
      lastTokenType = TOKEN_NUMBER;
      continue;
    }

    // Process operators
    if (IsOperator(ch))
    {
      token.type = TOKEN_OPERATOR;
      token.op = ch;
      TokenQueue_Add(token);
      lastTokenType = TOKEN_OPERATOR;
      i++;
      continue;
    }

    // Process left parenthesis
    if (ch == '(')
    {
      token.type = TOKEN_LPAREN;
      TokenQueue_Add(token);
      lastTokenType = TOKEN_LPAREN;
      i++;
      continue;
    }

    // Process right parenthesis
    if (ch == ')')
    {
      token.type = TOKEN_RPAREN;
      TokenQueue_Add(token);
      lastTokenType = TOKEN_RPAREN;
      i++;
      continue;
    }

    // Illegal character
    return CALC_ERR_SYNTAX;
  }

  return CALC_OK;
}

// ==================== Shunting Yard Algorithm (Infix to RPN) ====================

/**
 * Handle number token in Shunting Yard algorithm
 * Numbers are directly added to the output queue
 * @param token The number token to process
 * @param rpnOutput The RPN output array
 * @param rpnLen Pointer to the current length of RPN output
 */
static void HandleNumberToken(Token token, Token xdata *rpnOutput, unsigned char *rpnLen)
{
  rpnOutput[(*rpnLen)++] = token;
}

/**
 * Handle operator token in Shunting Yard algorithm
 * Pops operators with higher or equal precedence from stack to output
 * @param token The operator token to process
 * @param rpnOutput The RPN output array
 * @param rpnLen Pointer to the current length of RPN output
 */
static void HandleOperatorToken(Token token, Token xdata *rpnOutput, unsigned char *rpnLen)
{
  Token outputToken;
  char topOp;

  // Pop operators with precedence >= current operator
  while (!CharStack_IsEmpty())
  {
    topOp = CharStack_Peek();
    if (topOp == '(' || GetPrecedence(topOp) < GetPrecedence(token.op))
    {
      break;
    }

    outputToken.type = TOKEN_OPERATOR;
    outputToken.op = CharStack_Pop();
    rpnOutput[(*rpnLen)++] = outputToken;
  }

  // Push current operator onto stack
  CharStack_Push(token.op);
}

/**
 * Handle left parenthesis token in Shunting Yard algorithm
 * Left parenthesis is pushed onto the operator stack
 * @param parenCount Pointer to the parenthesis counter
 */
static void HandleLeftParenToken(unsigned char *parenCount)
{
  CharStack_Push('(');
  (*parenCount)++;
}

/**
 * Handle right parenthesis token in Shunting Yard algorithm
 * Pops operators until matching left parenthesis is found
 * @param rpnOutput The RPN output array
 * @param rpnLen Pointer to the current length of RPN output
 * @param parenCount Pointer to the parenthesis counter
 * @return CALC_OK or error code
 */
static unsigned char HandleRightParenToken(Token xdata *rpnOutput, unsigned char *rpnLen, unsigned char *parenCount)
{
  Token outputToken;
  char topOp;

  // Check for matching left parenthesis
  if (*parenCount == 0)
  {
    return CALC_ERR_SYNTAX; // Unmatched parenthesis
  }

  // Pop operators until left parenthesis is found
  while (!CharStack_IsEmpty())
  {
    topOp = CharStack_Pop();
    if (topOp == '(')
    {
      (*parenCount)--;
      break;
    }
    outputToken.type = TOKEN_OPERATOR;
    outputToken.op = topOp;
    rpnOutput[(*rpnLen)++] = outputToken;
  }

  return CALC_OK;
}

/**
 * Shunting Yard Algorithm: Convert token sequence to Reverse Polish Notation (RPN)
 * @return CALC_OK or error code
 */
static unsigned char ShuntingYard(void)
{
  unsigned char i;
  Token token;
  Token outputToken;
  char topOp;
  unsigned char parenCount = 0;
  Token xdata rpnOutput[MAX_TOKEN_QUEUE];
  unsigned char rpnLen = 0;
  unsigned char errCode;

  CharStack_Init();

  // Process each token
  for (i = 0; i < TokenQueue_Length(); i++)
  {
    token = *TokenQueue_Get(i);

    switch (token.type)
    {
    case TOKEN_NUMBER:
      HandleNumberToken(token, rpnOutput, &rpnLen);
      break;

    case TOKEN_OPERATOR:
      HandleOperatorToken(token, rpnOutput, &rpnLen);
      break;

    case TOKEN_LPAREN:
      HandleLeftParenToken(&parenCount);
      break;

    case TOKEN_RPAREN:
      errCode = HandleRightParenToken(rpnOutput, &rpnLen, &parenCount);
      if (errCode != CALC_OK)
      {
        return errCode;
      }
      break;

    default:
      return CALC_ERR_SYNTAX;
    }
  }

  // Pop all remaining operators from stack
  while (!CharStack_IsEmpty())
  {
    topOp = CharStack_Pop();
    if (topOp == '(')
    {
      return CALC_ERR_SYNTAX; // Unmatched parenthesis
    }
    outputToken.type = TOKEN_OPERATOR;
    outputToken.op = topOp;
    rpnOutput[rpnLen++] = outputToken;
  }

  // Check parenthesis balance
  if (parenCount != 0)
  {
    return CALC_ERR_SYNTAX;
  }

  // Copy RPN output to tokenQueue
  TokenQueue_Init();
  for (i = 0; i < rpnLen; i++)
  {
    TokenQueue_Add(rpnOutput[i]);
  }

  return CALC_OK;
}

// ==================== RPN Evaluation ====================

/**
 * RPN Evaluation: Evaluate the RPN token sequence
 * @return CALC_OK or error code
 */
static unsigned char EvaluateRPN(float *result)
{
  unsigned char i;
  Token token;
  float operand1, operand2;
  float opResult;
  unsigned char errCode;

  FloatStack_Init();

  for (i = 0; i < TokenQueue_Length(); i++)
  {
    token = *TokenQueue_Get(i);

    if (token.type == TOKEN_NUMBER)
    {
      // Push number onto stack
      if (FloatStack_IsFull())
      {
        return CALC_ERR_OVERFLOW;
      }
      FloatStack_Push(token.value);
    }
    else if (token.type == TOKEN_OPERATOR)
    {
      // Pop two operands
      if (FloatStack_Size() < 2)
      {
        return CALC_ERR_SYNTAX; // Insufficient operands
      }
      operand2 = FloatStack_Pop();
      operand1 = FloatStack_Pop();

      // Perform operation
      errCode = PerformOperation(token.op, operand1, operand2, &opResult);
      if (errCode != CALC_OK)
      {
        return errCode;
      }

      // Push result onto stack
      FloatStack_Push(opResult);
    }
  }

  // Stack should contain exactly one result
  if (FloatStack_Size() != 1)
  {
    return CALC_ERR_SYNTAX;
  }

  *result = FloatStack_Pop();
  return CALC_OK;
}

// ==================== Public Interface Functions ====================

void Calculator_Init(void)
{
  InitOperatorPrecedence();
  expressionLen = 0;
  expressionBuffer[0] = '\0';
}

unsigned char Calculator_InputChar(char ch)
{
  // Check if buffer is full
  if (expressionLen >= MAX_EXPR_LEN)
  {
    return 0;
  }

  // Check if character is valid
  if (!IsDigitOrDot(ch) && !IsOperator(ch) && ch != '(' && ch != ')')
  {
    return 0;
  }

  // Add character
  expressionBuffer[expressionLen++] = ch;
  expressionBuffer[expressionLen] = '\0';
  return 1;
}

void Calculator_Backspace(void)
{
  if (expressionLen > 0)
  {
    expressionLen--;
    expressionBuffer[expressionLen] = '\0';
  }
}

void Calculator_Clear(void)
{
  expressionLen = 0;
  expressionBuffer[0] = '\0';
}

char *Calculator_GetExpression(void)
{
  return expressionBuffer;
}

void Calculator_GetDisplayWindow(char *buffer, unsigned char offset)
{
  unsigned char i;
  unsigned char copyLen;

  // Validate offset
  if (offset > expressionLen)
  {
    offset = 0;
  }

  // Calculate how many characters to copy
  copyLen = expressionLen - offset;
  if (copyLen > LCD_DISPLAY_WIDTH)
  {
    copyLen = LCD_DISPLAY_WIDTH;
  }

  // Copy the window
  for (i = 0; i < copyLen; i++)
  {
    buffer[i] = expressionBuffer[offset + i];
  }

  // Fill remaining with spaces
  for (; i < LCD_DISPLAY_WIDTH; i++)
  {
    buffer[i] = ' ';
  }

  buffer[LCD_DISPLAY_WIDTH] = '\0';
}

unsigned char Calculator_GetMaxScrollOffset(void)
{
  if (expressionLen <= LCD_DISPLAY_WIDTH)
  {
    return 0;
  }
  return expressionLen - LCD_DISPLAY_WIDTH;
}

unsigned char Calculator_Evaluate(char *result)
{
  unsigned char errCode;
  float finalResult;

  // Empty expression
  if (expressionLen == 0)
  {
    strcpy(result, "");
    return CALC_OK;
  }

  // Lexical analysis
  errCode = Tokenize();
  if (errCode != CALC_OK)
  {
    strcpy(result, "Syntax error");
    return errCode;
  }

  // Shunting Yard algorithm (Infix to RPN)
  errCode = ShuntingYard();
  if (errCode != CALC_OK)
  {
    strcpy(result, "Syntax error");
    return errCode;
  }

  // RPN evaluation
  errCode = EvaluateRPN(&finalResult);
  if (errCode == CALC_ERR_DIV_ZERO)
  {
    strcpy(result, "Div by zero");
    return errCode;
  }
  else if (errCode != CALC_OK)
  {
    strcpy(result, "Syntax error");
    return errCode;
  }

  // Format result (5 decimal places)
  FloatToString(finalResult, result);

  return CALC_OK;
}
