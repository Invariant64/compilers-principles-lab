#include "tokens.h"
#include <stdio.h>
#include <assert.h>

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

static void print_keyword(int keyword_type) {
  assert(keyword_type < NR_KEYWORDS);
  printf("%s\n", keyword_type_str[keyword_type]);
}

void print_token(int token_type, char *value) {
  assert(token_type < NR_TOKENS);

  printf("%s\t", token_type_str[token_type]);
  switch (token_type) {
    case TK_SPACE: break;
    case TK_IDN:
    case TK_OCT:
    case TK_DEC:
    case TK_HEX:
    case TK_ILOCT:
    case TK_ILHEX:
      printf("%s\n", value);
      break;
    case TK_KEY:
      print_keyword(*value);
      break;
    default: printf("-\n");
  }
}

void print_tokens(struct token_t *tokens, int nr_tokens) {
  printf("Type\tValue\n");
  for (int i = 0; i < nr_tokens; i++) {
    print_token(tokens[i].type, tokens[i].value);
  }
}