#ifndef __FSM_H__
#define __FSM_H__

#include "tokens.h"

// fsm 识别以str为首的token, 并将结果存入token, 返回下一个token的首地址
char *fsm(char *str, struct token_t *token);

#endif // __FSM_H__