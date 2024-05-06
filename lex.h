#ifndef __LEX_H__
#define __LEX_H__

#include "fsm.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *lex_step(char *str, struct token_t *token);

#endif // __LEX_H__