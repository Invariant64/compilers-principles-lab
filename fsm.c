#include "fsm.h"
#include <stdio.h>
#include <assert.h>

char *fsm(char *str, struct token_t *token) {
  int idx = 0;
  char tmp_str[MAX_TOKEN_LEN] = {'\0'};
  int i = 0,a = 0;
  if(SPACE(str[idx]))
  {
    while(SPACE(str[idx]))
    {
      idx++;
    }
    token->type = TK_SPACE;
    //strcpy(token->value,NULL);
    return str + idx;
  }
  if(LETTER(str[idx]))
  {
    while(LETTER(str[idx]) || NUMBER(str[idx]))
    {
      tmp_str[i] = str[idx];
      i ++,idx ++;
    }
    if((a = RESERVEDWORD(tmp_str)) != -1) 
    {
      token->type = TK_IDN;
      strcpy(token->value, tmp_str);
    }
    else 
    {
      token->type = TK_KEY;
      sprintf(token->value,"%d",a); 
    }
    return str + idx;
  }
  if(NUMBER(str[idx]))
  {
    if(str[idx] != '0')
    {
      while(NUMBER(str[idx]))
      {
        tmp_str[i] = str[idx];
        i ++,idx ++;
      }
      token->type = TK_DEC;
      strcpy(token->value,tmp_str);
    }
    else 
    {
      tmp_str[i] = str[idx];
      i ++,idx ++;
      if(str[idx] == 'x' || str[idx] == 'X')
      {
        bool ilhex = false;
        tmp_str[i] = str[idx];
        i ++,idx ++;
        while(HEXNUM(str[idx]))
        {
          tmp_str[i] = str[idx];
          i ++,idx ++;
        }
        if((str[idx] >= 'g' && str[idx] <= 'z') || (str[idx] >= 'G' && str[idx] <= 'Z'))
        {
          ilhex = true;
          while(ILHEX(str[idx]))
          {
            tmp_str[i] = str[idx];
            i ++,idx ++;
          }
        }
        if(ilhex) token->type = TK_ILHEX;
        else token->type = TK_HEX;
        strcpy(token->value,tmp_str);
      }
      else if(OCTNUM(str[idx]))
      {
        bool iloct = false;
        while(OCTNUM(str[idx]))
        {
          tmp_str[i] = str[idx];
          i ++,idx ++;
        }
        if((str[idx] >= '8' && str[idx] <= '9'))
        {
          iloct = true;
          while(NUMBER(str[idx]))
          {
            tmp_str[i] = str[idx];
            i ++,idx ++;
          }
        }
        if(iloct) token->type = TK_ILOCT;
        else token->type = TK_OCT;
        strcpy(token->value,tmp_str);
      }
      else 
      {
        token->type = TK_DEC;
        strcpy(token->value,tmp_str);
      }
    }
    return str + idx;
  }
  if((a = PUCTUATION(str[idx])) != -1)
  {
    token->type = a;
    //strcpy(token->value,NULL);
    return str + idx + 1;
  }
  if((a = OPERATOR(str + idx)) != -1)
  {
    token->type = a;
    //strcpy(token->value,NULL);
    return str + idx + 1;
  }
  return NULL;
}

