#include "lex.h"

char *lex_step(char *str, struct token_t *token) {
  char invalid_stack[1024];
  int invalid_top = 0;
  bool in_invalid = false;

  char *p = str, *t;
  while (*p) {
    t = p;
    p = fsm(p, token);
    if (p == NULL) {
      if (!in_invalid) {
        in_invalid = true;
        invalid_top = 0;
      }
      invalid_stack[invalid_top++] = *t;
      p = t + 1;
    }
    else {
      if (in_invalid) {
        in_invalid = false;
        invalid_stack[invalid_top] = '\0';
        invalid_top = 0;
        token->type = TK_INVALID;
        strcpy(token->value, invalid_stack);
        return t;
      }
      return p;
    }
  }

  return NULL;
}