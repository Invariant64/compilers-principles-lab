#ifndef __LEX_H__
#define __LEX_H__

#include "fsm.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif	

char *lex_step(char *str, struct token_t *token);

#ifdef __cplusplus
}
#endif
#endif // __LEX_H__