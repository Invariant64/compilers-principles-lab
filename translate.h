#ifndef __TRANSLATE_H__
#define __TRANSLATE_H__

#include "procedures.h"


typedef struct variable_t {
  enum VariableType type;
  char *value;

  variable_t* sons;
  variable_t* father;
  variable_t* brothers;
} variable_t;

void display_tree(struct variable* node);

#endif // __TRANSLATE_H__