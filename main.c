#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fsm.h"

#define DEFAULT_INPUT "07 0x5f 0x1h 0g"

int main(int argc, char **argv) {
  char *str;
  char *input_file;

  if (argc == 1) {
    str = (char *)malloc(strlen(DEFAULT_INPUT) + 1);
    strcpy(str, DEFAULT_INPUT);
  }
  else {
    input_file = argv[1];
    FILE *fp = fopen(input_file, "r");
    if (fp == NULL) {
      fprintf(stderr, "Error: Cannot open file %s\n", input_file);
      return 1;
    }

    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    str = (char *)malloc(fsize + 1);
    fread(str, 1, fsize, fp);
    fclose(fp);
    str[fsize] = '\0';
  }

  struct token_t tokens[1024];
  int nr_tokens = 0;

  char invalid_stack[1024];
  int invalid_top = 0;
  bool in_invalid = false;

  char *p = str;
  char *t;
  while (*p) {
    t = p;
    p = fsm(p, &tokens[nr_tokens]);
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
        printf("Invalid token: %s\n", invalid_stack);
      }
      if (tokens[nr_tokens].type != TK_SPACE) nr_tokens++;
    }
  }

  if (in_invalid) {
    in_invalid = false;
    invalid_stack[invalid_top] = '\0';
    printf("Invalid token: %s\n", invalid_stack);
  }

  print_tokens(tokens, nr_tokens);

  free(str);

  return 0;
}