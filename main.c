#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lex.h"
#include"procedures.h"

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

  char *p = str;
  while (*p) {
    p = lex_step(p, &tokens[nr_tokens]);
    if (p == NULL) 
      break;
    else if (tokens[nr_tokens].type != TK_SPACE)
      nr_tokens++;
  }
  
  print_tokens(tokens, nr_tokens);

  struct variable* P_node = (struct variable*)malloc(sizeof(struct variable));
  strcpy(P_node->name, "P");
  P_node->brothers = NULL;
  P_node->father = NULL;
  P_node->sons = NULL;

  int temp_index = 0;

  if (P(tokens, &temp_index, nr_tokens, P_node) == 1) {
      printf("分析成功！\n");
  }
  else {
      printf("分析失败\n");
  }
  
  print_tree(P_node);

  free(str);

  return 0;
}