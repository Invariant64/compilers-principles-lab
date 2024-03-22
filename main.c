#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fsm.h"

#define DEFAULT_INPUT "1 + 2 * 3"

int main(int argc, char **argv) {
  char *str;
  char *input_file;

  if (argc == 1) {
    str = (char *)malloc(strlen(DEFAULT_INPUT) + 1);
    strcpy(str, DEFAULT_INPUT);
  } else {
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
    str[fsize] = 0;
  }

  struct token_t tokens[1024];
  int nr_tokens = 0;

  char *p = str;
  while (*p) {
    p = fsm(p, &tokens[nr_tokens]);
    nr_tokens++;
  }

  print_tokens(tokens, nr_tokens);

  free(str);

  return 0;
}