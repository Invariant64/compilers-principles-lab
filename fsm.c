#include "fsm.h"
#include <stdio.h>
#include <assert.h>
#include "string.h"

enum State {
  S1, S2, S3, S4, S5, S6, S7, S8, S9, S10,
  S11, S12, S13, S14, S15, S16, S17, S18, S19, S20,
  S21,
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
      if (NUMBER(cur, 0, 9)) return S4;
      if (ALETTER(cur)) return S4;
      if (XLETTER(cur, 'g', 'z')) return S18;
      return INVALID_STATE;
    case S8:
      if (NUMBER(cur, 0, 9)) return S8;
      return S9;
    case S20:
      if (SPACE(cur)) return S20;
      return S21;
    // TODO: more states' transfer
  }
  return -1;
}

char *fsm(char *str, struct token_t *token) {
  int state = S1;
  char *cur = str;
  token->type = -1;

  while (1) {
    // TODO: terminal states 
    switch (state) {
      case S5:  token->type = TK_HEX; break;
      case S7:  token->type = TK_OCT; break;
      case S9:  token->type = TK_DEC; break;
      case S17: token->type = TK_ILOCT; break;
      case S19: token->type = TK_ILHEX; break;
      case S13: 
      case S15:
      case S21: token->type = TK_SPACE; break;
      case -1: return NULL;
    }
    if (token->type != -1) break;
    state = state_transfer(state, *cur);
    cur++;
  }

  switch (state) {
    case S5: 
    case S7: 
    case S9: 
    case S17: 
    case S19: 
      strncpy(token->value, str, cur - str);
      token->value[cur - str] = '\0';
  }
  
  return cur - 1;
}

// char *fsm(char *str, struct token_t *token) {
//   int idx = 0;
//   char tmp_str[MAX_TOKEN_LEN] = {'\0'};
//   int i = 0,a = 0;
//   if(SPACE(str[idx]))
//   {
//     while(SPACE(str[idx]))
//     {
//       idx++;
//     }
//     token->type = TK_SPACE;
//     //strcpy(token->value,NULL);
//     return str + idx;
//   }
//   if(LETTER(str[idx]))
//   {
//     while(LETTER(str[idx]) || NUMBER(str[idx]))
//     {
//       tmp_str[i] = str[idx];
//       i ++,idx ++;
//     }
//     if((a = RESERVEDWORD(tmp_str)) != -1) 
//     {
//       token->type = TK_IDN;
//       strcpy(token->value, tmp_str);
//     }
//     else 
//     {
//       token->type = TK_KEY;
//       sprintf(token->value,"%d",a); 
//     }
//     return str + idx;
//   }
//   if(NUMBER(str[idx]))
//   {
//     if(str[idx] != '0')
//     {
//       while(NUMBER(str[idx]))
//       {
//         tmp_str[i] = str[idx];
//         i ++,idx ++;
//       }
//       token->type = TK_DEC;
//       strcpy(token->value,tmp_str);
//     }
//     else 
//     {
//       tmp_str[i] = str[idx];
//       i ++,idx ++;
//       if(str[idx] == 'x' || str[idx] == 'X')
//       {
//         bool ilhex = false;
//         tmp_str[i] = str[idx];
//         i ++,idx ++;
//         while(HEXNUM(str[idx]))
//         {
//           tmp_str[i] = str[idx];
//           i ++,idx ++;
//         }
//         if((str[idx] >= 'g' && str[idx] <= 'z') || (str[idx] >= 'G' && str[idx] <= 'Z'))
//         {
//           ilhex = true;
//           while(ILHEX(str[idx]))
//           {
//             tmp_str[i] = str[idx];
//             i ++,idx ++;
//           }
//         }
//         if(ilhex) token->type = TK_ILHEX;
//         else token->type = TK_HEX;
//         strcpy(token->value,tmp_str);
//       }
//       else if(OCTNUM(str[idx]))
//       {
//         bool iloct = false;
//         while(OCTNUM(str[idx]))
//         {
//           tmp_str[i] = str[idx];
//           i ++,idx ++;
//         }
//         if((str[idx] >= '8' && str[idx] <= '9'))
//         {
//           iloct = true;
//           while(NUMBER(str[idx]))
//           {
//             tmp_str[i] = str[idx];
//             i ++,idx ++;
//           }
//         }
//         if(iloct) token->type = TK_ILOCT;
//         else token->type = TK_OCT;
//         strcpy(token->value,tmp_str);
//       }
//       else 
//       {
//         token->type = TK_DEC;
//         strcpy(token->value,tmp_str);
//       }
//     }
//     return str + idx;
//   }
//   if((a = PUNCTUATION(str[idx])) != -1)
//   {
//     token->type = a;
//     //strcpy(token->value,NULL);
//     return str + idx + 1;
//   }
//   if((a = OPERATOR(str + idx)) != -1)
//   {
//     token->type = a;
//     //strcpy(token->value,NULL);
//     return str + idx + 1;
//   }
//   return NULL;
// }

