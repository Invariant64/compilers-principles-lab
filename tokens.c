#include "tokens.h"
#include <stdio.h>
#include <assert.h>

const char *token_type_str[] = {
  "SPACE", "IDN", "OCT", "DEC", "HEX", "ADD", "SUB", "MUL", "DIV", 
  "GT", "LT", "GE", "LE", "EQ", "NEQ", "SLP", "SRP", "SEM", "KEY", 
  "ILOCT", "ILHEX", "OTHER"
};

const char *keyword_type_str[] = {
  "IF", "THEN", "ELSE", "WHILE", "DO", "BEGIN", "END"
};

#define NR_TOKENS (sizeof(token_type_str) / sizeof(token_type_str[0]))
#define NR_KEYWORDS (sizeof(keyword_type_str) / sizeof(keyword_type_str[0]))

bool SPACE(char c) {
  return c == ' ' || c == '\t' || c == '\n';
}
bool NUMBER(char c) {
  return c >= '0' && c <= '9';
}
bool OCTNUM(char c)
{
  return c >= '0' && c <= '7';
}
bool HEXNUM(char c)
{
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}
bool ILHEX(char c)
{
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
bool LETTER(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
int PUCTUATION(char c)
{
  switch (c) {
  case '(': return TK_SLP; break;
    case ')': return TK_SRP;break;
    case ';': return TK_SEM;break;
    default: return -1;
  }
  return -1;
}
int OPERATOR(char *c)
{
  switch (*c) {
    case '+': 
      return TK_ADD;
      break;
    case '-': 
      return TK_SUB;
      break;
    case '*': 
      return TK_MUL;
      break;
    case '/': 
      return TK_DIV;
      break;
    case '>': 
      if(*(c + 1) == '=') return TK_GE;
      else return TK_GT;
      break;
    case '<': 
      if(*(c + 1) == '=') return TK_LE;
      else return TK_LT;
      break;
    case '=': 
      return TK_EQ;
      break;
    case '!': 
      if(*(c + 1) == '=') return TK_NEQ;
      else return -1;
      break;
    default: return -1;
  }
  return -1;
}
int RESERVEDWORD(char *c)
{
  switch (*c) {
    case 'i':
      if (strcmp(c, "if") == 0) return KW_IF;
      break;
    case 't':
      if (strcmp(c, "then") == 0) return KW_THEN;
      break;
    case 'e':
      if (strcmp(c, "else") == 0) return KW_ELSE;
      else if (strcmp(c, "end") == 0) return KW_END;
      break;
    case 'w':
      if (strcmp(c, "while") == 0) return KW_WHILE;
      break;
    case 'd':
      if (strcmp(c, "do") == 0) return KW_DO;
      break;
    case 'b':
      if (strcmp(c, "begin") == 0) return KW_BEGIN;
      break;
    default: return -1;
  }
  return -1;
}


static void print_keyword(int keyword_type) {
  assert(keyword_type < NR_KEYWORDS);
  printf("%s\n", keyword_type_str[keyword_type]);
}

void print_token(int token_type, char *value) {
  assert(token_type < NR_TOKENS);
  if (token_type != TK_SPACE)
  {
      printf("%s\t", token_type_str[token_type]);
      switch (token_type) {
        //case TK_SPACE: break;
        case TK_IDN:
        case TK_OCT:
        case TK_DEC:
        case TK_HEX:
        case TK_ILOCT:
        case TK_ILHEX:
          printf("%s\n", value);
          break;
        case TK_KEY:
          print_keyword(atoi(value));
          break;
        default: printf("-\n");
      }
  }
}

void print_tokens(struct token_t *tokens, int nr_tokens) {
  printf("Type\tValue\n");
  for (int i = 0; i < nr_tokens; i++) {
    print_token(tokens[i].type, tokens[i].value);
  }
}