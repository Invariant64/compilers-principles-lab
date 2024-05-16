#ifndef __TOKENS_H__
#define __TOKENS_H__

#define MAX_TOKEN_LEN 64
#include <stdbool.h>
enum token_type {
  TK_SPACE,  // Space
  TK_IDN,    // Identifier
  TK_OCT,    // Octal
  TK_DEC,    // Decimal
  TK_HEX,    // Hexadecimal
  TK_ADD,    // Addition
  TK_SUB,    // Subtraction
  TK_MUL,    // Multiplication
  TK_DIV,    // Division
  TK_GT,     // Greater than
  TK_LT,     // Less than
  TK_GE,     // Greater than or equal to
  TK_LE,     // Less than or equal to
  TK_EQ,     // Equal to
  TK_NEQ,    // Not equal to
  TK_SLP,    // Left parenthesis
  TK_SRP,    // Right parenthesis
  TK_SEM,    // Semicolon
  TK_KEY,    // Keyword
  TK_ILOCT,  // Invalid octal
  TK_ILHEX,  // Invalid hexadecimals
  TK_OTHER,  // Other
  TK_INVALID // Invalid
};

enum keyword_type {
  KW_IF,    // If
  KW_THEN,  // Then
  KW_ELSE,  // Else
  KW_WHILE, // While
  KW_DO,    // Do
  KW_BEGIN, // Begin
  KW_END,   // End
};

extern const char* keyword_type_str[];

struct token_t {
  int type;
  char value[MAX_TOKEN_LEN];
};

void print_token(int token_type, char *value);
void print_tokens(struct token_t *tokens, int nr_tokens);

bool SPACE(char c);
bool NUMBER(char c, int from, int to);
bool LETTER(char c, char from, char to);
bool XLETTER(char c, char from, char to);
bool ALETTER(char c);
bool OCTNUM(char c);
bool HEXNUM(char c);
bool OPERATOR(char c);
int OP_TYPE(char *c);
bool OTHERS(char c, const char *s);
bool KEYWORD(char *c, int len);
int KEYWORD_TYPE(char *c, int len);

#endif // __TOKENS_H__