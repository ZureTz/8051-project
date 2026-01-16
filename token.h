#ifndef TOKEN_H
#define TOKEN_H

// Token type definitions
#define TOKEN_NUMBER 0   // Number
#define TOKEN_OPERATOR 1 // Operator (+, -, *, /)
#define TOKEN_LPAREN 2   // Left parenthesis (
#define TOKEN_RPAREN 3   // Right parenthesis )

// Operator definitions
#define OP_ADD '+'
#define OP_SUB '-'
#define OP_MUL '*'
#define OP_DIV '/'

// Token structure
typedef struct
{
  unsigned char type; // TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_LPAREN, TOKEN_RPAREN
  float value;        // Numeric value (valid only when type == TOKEN_NUMBER)
  char op;            // Operator (valid only when type == TOKEN_OPERATOR)
} Token;

#endif // TOKEN_H