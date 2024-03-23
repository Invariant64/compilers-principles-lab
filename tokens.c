#include "tokens.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "string.h"

const char *token_type_str[] = {
  "SPACE", "IDN", "OCT", "DEC", "HEX", "ADD", "SUB", "MUL", "DIV", 
  "GT", "LT", "GE", "LE", "EQ", "NEQ", "SLP", "SRP", "SEM", "KEY", 
  "ILOCT", "ILHEX"
};

const char *keyword_type_str[] = {
  "IF", "THEN", "ELSE", "WHILE", "DO", "BEGIN", "END"
};

#define NR_TOKENS (sizeof(token_type_str) / sizeof(token_type_str[0]))
#define NR_KEYWORDS (sizeof(keyword_type_str) / sizeof(keyword_type_str[0]))

bool SPACE(char c) {
  return c == ' ' || c == '\t' || c == '\n';
}

bool NUMBER(char c, int from, int to) {
  return c >= from + '0' && c <= to + '0';
}

bool LETTER(char c, char from, char to) {
  return c >= from && c <= to;
}

bool XLETTER(char c, char from, char to) {
  if (from <= 'z' && from >= 'a') {
    return LETTER(c, from, to) || LETTER(c, from - 'a' + 'A', to - 'a' + 'A');
  } else if (from <= 'Z' && from >= 'A') {
    return LETTER(c, from, to) || LETTER(c, from - 'A' + 'a', to - 'A' + 'a');
  }
  return false;
}

bool ALETTER(char c) {
  return XLETTER(c, 'a', 'z');
}

bool OCTNUM(char c) {
  return NUMBER(c, 0, 7);
}

bool HEXNUM(char c) {
  return NUMBER(c, 0, 9) || LETTER(c, 'a', 'f');
}

bool OTHERS(char c, const char *s) {
  for (int i = 0; s[i]; i++) {
    if (c == s[i]) return true;
  }
  return false;
}

bool OPERATOR(char c) {
  switch (c) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '>': 
    case '<':
    case '=':
    case '(':
    case ')':
    case ';':
      return true;
    default: return false;
  }
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