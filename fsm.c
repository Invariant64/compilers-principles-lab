#include "fsm.h"
#include <stdio.h>
#include <assert.h>
#include "string.h"

enum State {
  S1, S2, S3, S4, S5, S6, S7, S8, S9, S10,
  S11, S12, S13, S14, S15, S16, S17, S18, S19, S20,
  S21, S22,
  INVALID_STATE
};

int state_transfer(int state, char cur) {
  switch (state) {
    case S1:
      if (cur == '0')        return S2;
      if (NUMBER(cur, 1, 9)) return S8;
      if (OPERATOR(cur))     return S10;
      if (cur == '>')        return S11;
      if (cur == '<')        return S12;
      if (ALETTER(cur))      return S14;
      if (SPACE(cur))        return S20;
      return INVALID_STATE;
    case S2:
      if (NUMBER(cur, 8, 9)) return S16;
      if (NUMBER(cur, 0, 7)) return S6;
      if (OTHERS(cur, "xX")) return S3;
      return S9;
    case S3:
      if (NUMBER(cur, 0, 9) || XLETTER(cur, 'a', 'f')) return S4;
      if (XLETTER(cur, 'g', 'z')) return S18;
      return INVALID_STATE;
    case S4:
      if (XLETTER(cur, 'a', 'f') || NUMBER(cur, 0, 9)) return S4;
      if (XLETTER(cur, 'g', 'z')) return S18;
      return S5;
    case S6:
      if (NUMBER(cur, 0, 7)) return S6;
      if (NUMBER(cur, 8, 9)) return S16;
      return S7;
    case S8:
      if (NUMBER(cur, 0, 9)) return S8;
      return S9;
    case S10:
      return S13;
    case S11:
      if (cur == '=')     return S10;
      else                return S13;
    case S12:
      if (cur == '=')     return S10;
      else if (cur == '>') return S10;
      else                return S13;
    case S14:
      if (ALETTER(cur) || NUMBER(cur, 0, 9)) return S14;
      return S15;
    case S16:
      if (NUMBER(cur, 0, 9)) return S16;
      return S17;
    case S18:
      if (XLETTER(cur, 'a', 'z') || NUMBER(cur, 0, 9)) return S18;
      return S19;
    case S20:
      if (SPACE(cur)) return S20;
      return S21;
  }
  return -1;
}

char *fsm(char *str, struct token_t *token) {
  int state = S1;
  char *cur = str;
  token->type = -1;
  while (1) {
    switch (state) {
      case S5:  token->type = TK_HEX; break;
      case S7:  token->type = TK_OCT; break;
      case S9:  token->type = TK_DEC; break;
      case S17: token->type = TK_ILOCT; break;
      case S19: token->type = TK_ILHEX; break;
      case S13: token->type = OP_TYPE(str); break;
      case S15: token->type = KEYWORD(str, cur - str - 1) ? TK_KEY : TK_IDN; break;
      case S21: token->type = TK_SPACE; break;
      case INVALID_STATE: return NULL;
    }
    if (token->type != -1) break;
    state = state_transfer(state, *cur);
    cur++;
  }
  cur--;
  switch (state) {
    case S15:
      if (token->type == TK_KEY) {
        sprintf(token->value, "%d", KEYWORD_TYPE(str, cur - str));
        break;
      }
    case S5:
    case S7:
    case S9:
    case S13:
    case S17:
    case S19:
      strncpy(token->value, str, cur - str);
      token->value[cur - str] = '\0';
  }

  return cur;
}