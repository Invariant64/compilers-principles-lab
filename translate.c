#include "procedures.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

struct {
  enum VariableType type;
  const char *value;
} v_identyfier_type_str[] = {
  { VT_P, "P" },
  { VT_S, "S" },
  { VT_A, "A" },
  { VT_H, "H" },
  { VT_C, "C" },
  { VT_E, "E" },
  { VT_J, "J" },
  { VT_B, "B" },
  { VT_D, "D" },
  { VT_F, "F" },
  { VT_T, "T" }
}, v_simbol_type_str[] = {
  { VT_ADD, "+" },
  { VT_SUB, "-" },
  { VT_MUL, "*" },
  { VT_DIV, "/" },
  { VT_SLP, "(" },
  { VT_SRP, ")" },
  { VT_SEM, ";" },
  { VT_GT , ">" },
  { VT_LT , "<" },
  { VT_EQ , "=" }
}, v_keyword_type_str[] = {
  { VT_IF   , "IF"    },
  { VT_THEN , "THEN"  },
  { VT_ELSE , "ELSE"  },
  { VT_WHILE, "WHILE" },
  { VT_DO   , "DO"    }
};

#define NR_IDENTIFIERS (sizeof(v_identyfier_type_str) / sizeof(v_identyfier_type_str[0]))
#define NR_SIMBOLS (sizeof(v_simbol_type_str) / sizeof(v_simbol_type_str[0]))
#define NR_KEYWORDS (sizeof(v_keyword_type_str) / sizeof(v_keyword_type_str[0]))

void init_variable(struct variable *node) {
  for (int i = 0; i < NR_IDENTIFIERS; i++) {
    if (strcmp(node->name, v_identyfier_type_str[i].value) == 0 && node->sons != NULL) {
      node->type = v_identyfier_type_str[i].type;
      return;
    }
  }
  for (int i = 0; i < NR_SIMBOLS; i++) {
    if (strcmp(node->name, v_simbol_type_str[i].value) == 0) {
      node->type = v_simbol_type_str[i].type;
      return;
    }
  }
  for (int i = 0; i < NR_KEYWORDS; i++) {
    if (strcmp(node->name, v_keyword_type_str[i].value) == 0) {
      node->type = v_keyword_type_str[i].type;
      return;
    }
  }
  if (strcmp(node->name, "ε") == 0) {
    node->type = VT_EMPTY;
  } else if (node->name[0] == '0' && node->name[1] == 'x') {
    node->type = VT_INT16;
  } else if (node->name[0] == '0') {
    node->type = VT_INT8;
  } else if (node->name[0] > '0' && node->name[0] <= '9') {
    node->type = VT_INT10;
  } else {
    node->type = VT_ID;
  }
}

void cal_type(struct variable *node) {
  if (node == NULL) return;
  init_variable(node);
  struct variable *p = node->sons;
  while (p) {
    cal_type(p);
    p = p->brothers;
  }
}

void display_variable(struct variable *node) {
  if (node == NULL) return;
  printf("%d\t%s\n", node->type, node->name);
  struct variable *p = node->sons;
  while (p) {
    display_variable(p);
    p = p->brothers;
  }
}

void display_tree(struct variable *node) {
  cal_type(node);
  display_variable(node);
}

char *char_concat(int n, ...) {
  va_list args;

  int len = 0;
  va_start(args, n);
  char *ss;
  for (int i = 0; i < n; i++) {
    ss = va_arg(args, char *);
    if (ss == NULL) continue;
    len += strlen(ss);
  }
  va_end(args);

  char *s = (char *)malloc(len + 1);
  va_start(args, n);
  for (int i = 0; i < n; i++) {
    ss = va_arg(args, char *);
    if (ss == NULL) continue;
    strcat(s, ss);
  }
  va_end(args);

  return s;
}

struct {
  int id;
  int num_sons;
  enum VariableType variable;
  const enum VariableType sons[5];
} productions[] = {
  { 0 , 3, VT_P, { VT_S, VT_SEM, VT_A               } },
  { 1 , 1, VT_A, { VT_P                             } },
  { 2 , 1, VT_A, { VT_EMPTY                         } },
  { 3 , 3, VT_S, { VT_ID, VT_EQ, VT_E               } },
  { 4 , 5, VT_S, { VT_IF, VT_C, VT_THEN, VT_S, VT_H } },
  { 5 , 3, VT_H, { VT_SEM, VT_ELSE, VT_S            } },
  { 6 , 1, VT_H, { VT_EMPTY                         } },
  { 7 , 4, VT_S, { VT_WHILE, VT_C, VT_DO, VT_S      } },
  { 8 , 2, VT_C, { VT_E, VT_J                       } },
  { 9 , 2, VT_J, { VT_GT, VT_E                      } },
  { 10, 2, VT_J, { VT_LT, VT_E                      } },
  { 11, 2, VT_J, { VT_EQ, VT_E                      } },
  { 12, 2, VT_E, { VT_T, VT_B                       } },
  { 13, 3, VT_B, { VT_ADD, VT_T, VT_B               } },
  { 14, 3, VT_B, { VT_SUB, VT_T, VT_B               } },
  { 15, 1, VT_B, { VT_EMPTY                         } },
  { 16, 2, VT_T, { VT_F, VT_D                       } },
  { 17, 3, VT_D, { VT_MUL, VT_F, VT_D               } },
  { 18, 3, VT_D, { VT_DIV, VT_F, VT_D               } },
  { 19, 1, VT_D, { VT_EMPTY                         } },
  { 20, 3, VT_F, { VT_SLP, VT_E, VT_SRP             } },
  { 21, 1, VT_F, { VT_ID                            } },
  { 22, 1, VT_F, { VT_INT8                          } },
  { 23, 1, VT_F, { VT_INT10                         } },
  { 24, 1, VT_F, { VT_INT16                         } }
};

#define NR_PRODUCTIONS (sizeof(productions) / sizeof(productions[0]))

char *new_temp() {
  static int temp_index = 0;
  char *temp = (char *)malloc(4);
  sprintf(temp, "t%d", temp_index++);
  return temp;
}

char *new_label() {
  static int label_index = 0;
  char *label = (char *)malloc(4);
  sprintf(label, "L%d", label_index++);
  return label;
}

static int zipper_index = 0;
static char **zipper[1024] = { 0 };
int zipper_push(char **label) {
  zipper[zipper_index++] = label;
  return zipper_index - 1;
}

void action(struct variable *node, struct variable *sons[], int type) {
  switch (type) {
    case 0:
      // P -> S ; A
      printf("P -> S ; A\n");
      node->info.code = char_concat(2, sons[0]->info.code, sons[2]->info.code);

      printf("0: P -> S ; A\nP->code: %s\n", node->info.code);
      break;
    case 1:
      // A -> P
      printf("A -> P\n");
      node->info.code = sons[0]->info.code;

      printf("1: A -> P\nA->code: %s\n", node->info.code);
      break;
    case 2:
      // A -> ε
      printf("A -> ε\n");
      node->info.code = (char *)malloc(1);
      node->info.code[0] = '\0';

      printf("2: A -> ε\nA->code: %s\n", node->info.code);
      break;
    case 3:
      // S -> ID = E
      printf("S -> ID := E\n");
      sons[0]->info.place = (char *)malloc(7);
      strcpy(sons[0]->info.place, sons[0]->name);
      node->info.code = char_concat(6, sons[2]->info.code, "\t", sons[0]->info.place, " := ", sons[2]->info.place, "\n");
      
      printf("3: S -> ID := E\nS->code: %s\n", node->info.code);
      break;
    case 4:
      // S -> IF C THEN S H
      printf("S -> IF C THEN S H\n");

      if (sons[4]->sons->type == VT_SEM) {
        // sons[1]->info.fexit = new_label();
        if (sons[3]->info.begin == NULL) {
          sons[1]->info.texit = sons[3]->info.begin = new_label();
          node->info.code = char_concat(5, sons[1]->info.code, sons[1]->info.texit, ":\n", sons[3]->info.code, "\tgoto ");
        } else {
          sons[1]->info.texit = sons[3]->info.begin;
          node->info.code = char_concat(3, sons[1]->info.code, sons[3]->info.code, "\tgoto ");
        }

        if (sons[4]->info.begin == NULL) {
          sons[1]->info.fexit = sons[4]->info.begin = new_label();
          node->info.code = char_concat(6, node->info.code, sons[1]->info.fexit, "\n", sons[1]->info.fexit, ":\n", sons[4]->info.code);
        } else {
          sons[1]->info.fexit = sons[4]->info.begin;
          node->info.code = char_concat(4, node->info.code, sons[1]->info.fexit, "\n", sons[4]->info.code);
        }

        if (sons[3]->info.next == NULL && sons[4]->info.next == NULL) {
          node->info.next = new_label();
          node->info.code = char_concat(3, node->info.code, node->info.next, ":\n");
        } else if (sons[3]->info.next != NULL) {
          node->info.next = sons[3]->info.next;
        } else {
          node->info.next = sons[4]->info.next;
        }
      } else {
        if (sons[3]->info.begin == NULL) {
          sons[1]->info.texit = sons[3]->info.begin = new_label();
          node->info.code = char_concat(4, sons[1]->info.code, sons[1]->info.texit, ":\n", sons[3]->info.code);
        } else {
          sons[1]->info.texit = sons[3]->info.begin;
          node->info.code = char_concat(2, sons[1]->info.code, sons[3]->info.code);
        }
        if (sons[3]->info.next == NULL) {
          sons[1]->info.fexit = node->info.next = new_label();
          node->info.code = char_concat(3, node->info.code, node->info.next, ":\n");
        } else {
          sons[1]->info.fexit = node->info.next = sons[3]->info.next;
        }
      }
      break;
    case 5:
      // H -> ; ELSE S
      printf("H -> ; ELSE S\n");
      sons[2]->info.next = node->info.next;
      node->info.code = sons[2]->info.code;
      node->info.begin = sons[2]->info.begin;
      break;
    case 6:
      // H -> ε
      printf("H -> ε\n");
      node->info.code = (char *)malloc(1);
      node->info.code[0] = '\0';

      printf("6: H -> ε\nH->code: %s\n", node->info.code);
      break;
    case 7:
      // S -> WHILE C DO S
      printf("S -> WHILE C DO S\n");
      node->info.begin = new_label();
      sons[1]->info.texit = new_label();
      node->info.next = new_label();
      sons[1]->info.fexit = node->info.next;
      sons[3]->info.next = node->info.begin;
      node->info.code = char_concat(11, node->info.begin, ":\n", sons[1]->info.code, sons[1]->info.texit, ":\n", 
        sons[3]->info.code, "\tgoto ", node->info.begin, "\n", node->info.next, ":\n");
      break;
    case 8:
      // C -> E J
      printf("C -> E J\n");
      char tempt[10], tempf[10];
      sprintf(tempt, "{{%d}}", zipper_push(&node->info.texit));
      sprintf(tempf, "{{%d}}", zipper_push(&node->info.fexit));
      node->info.code = char_concat(12, sons[0]->info.code, sons[1]->info.code, "\tif ", sons[0]->info.place, 
        sons[1]->info.op, sons[1]->info.place, " goto ", tempt, "\n", "\tgoto ", tempf, "\n");
      break;
    case 9:
      // J -> > E
    case 10:
      // J -> < E
    case 11:
      // J -> = E
      printf("J -> %s E\n", sons[0]->name);
      node->info.code = sons[1]->info.code;
      node->info.place = sons[1]->info.place;
      node->info.op = sons[0]->name;

      printf("9-11: J -> %s E\nJ->code: %s\nJ->place: %s\nJ->op: %s\n", sons[0]->name, node->info.code, node->info.place, node->info.op);
      break;
    case 12:
      // E -> T B
      printf("E -> T B\n");
      if (sons[1]->sons->type == VT_EMPTY) {
        node->info.place = sons[0]->info.place;
        node->info.code = sons[0]->info.code;
      } else {
        node->info.place = new_temp();
        node->info.code = char_concat(9, sons[0]->info.code, sons[1]->info.code, "\t", node->info.place, " := ", 
          sons[0]->info.place, sons[1]->info.op, sons[1]->info.place, "\n");
      }
      
      printf("12: E -> T B\nE->place: %s\nE->code: %s\n", node->info.place, node->info.code);
      break;
    case 13:
      // B -> + T B
    case 14:
      // B -> - T B
      printf("B -> %s T B\n", sons[0]->name);
      node->info.op = sons[0]->name;
      if (sons[2]->sons->type == VT_EMPTY) {
        node->info.place = sons[1]->info.place;
        node->info.code = sons[1]->info.code;
      } else {
        node->info.place = new_temp();
        node->info.code = char_concat(9, sons[1]->info.code, sons[2]->info.code, "\t", node->info.place, " := ", 
          sons[1]->info.place, sons[2]->info.op, sons[2]->info.place, "\n");
      }
      break;
    case 15:
      // B -> ε
      printf("B -> ε\n");
      node->info.code = (char *)malloc(1);
      node->info.code[0] = '\0';

      printf("15: B -> ε\nB->code: %s\n", node->info.code);
      break;
    case 16:
      // T -> F D
      printf("T -> F D\n");
      if (sons[1]->sons->type == VT_EMPTY) {
        node->info.place = sons[0]->info.place;
        node->info.code = sons[0]->info.code;
      } else {
        node->info.place = new_temp();
        node->info.code = char_concat(9, sons[0]->info.code, sons[1]->info.code, "\t", node->info.place, " := ", 
          sons[0]->info.place, sons[1]->info.op, sons[1]->info.place, "\n");
      }

      printf("16: T -> F D\nT->place: %s\nT->code: %s\n", node->info.place, node->info.code);
      break;
    case 17:
      // D -> * F D
    case 18:
      // D -> / F D
      printf("D -> %s F D\n", sons[0]->name);
      node->info.op = sons[0]->name;
      if (sons[2]->sons->type == VT_EMPTY) {
        node->info.place = sons[1]->info.place;
        node->info.code = sons[1]->info.code;
      } else {
        node->info.place = new_temp();
        node->info.code = char_concat(9, sons[1]->info.code, sons[2]->info.code, "\t", node->info.place, " := ", 
          sons[1]->info.place, sons[2]->info.op, sons[2]->info.place, "\n");
      }
      break;
    case 19:
      // D -> ε
      printf("D -> ε\n");
      node->info.code = (char *)malloc(1);
      node->info.code[0] = '\0';

      printf("19: D -> ε\nD->code: %s\n", node->info.code);
      break;
    case 20:
      // F -> ( E )
      printf("F -> ( E )\n");
      node->info.place = sons[1]->info.place;
      node->info.code = sons[1]->info.code;

      printf("20: F -> ( E )\nF->place: %s\nF->code: %s\n", node->info.place, node->info.code);
      break;
    case 21:
      // F -> ID
      node->info.place = (char *)malloc(7);
      strcpy(node->info.place, sons[0]->name);
      node->info.code = (char *)malloc(1);
      node->info.code[0] = '\0';
      break;
    case 22: 
      // F -> INT8
      node->info.place = (char *)malloc(7);
      sprintf(node->info.place, "%d", strtol(sons[0]->name, NULL, 8));
      node->info.code = (char *)malloc(1);
      node->info.code[0] = '\0';
      break;
    case 23:
      // F -> INT10
      node->info.place = (char *)malloc(7);
      sprintf(node->info.place, "%d", strtol(sons[0]->name, NULL, 10));
      node->info.code = (char *)malloc(1);
      node->info.code[0] = '\0';
      break;
    case 24:
      // F -> INT16
      printf("F -> %s\n", sons[0]->name);
      node->info.place = (char *)malloc(7);
      sprintf(node->info.place, "%ld", strtol(sons[0]->name, NULL, 16));
      node->info.code = (char *)malloc(1);
      node->info.code[0] = '\0';

      printf("21-24: F -> %s\nF->place: %s\nF->code: %s\n", sons[0]->name, node->info.place, node->info.code);
      break;
    default:
      assert(0);
  }
}

void translate(struct variable *node) {
  if (node == NULL) return;
  for (int i = 0; i < NR_PRODUCTIONS; i++) {
    if (node->type == productions[i].variable) {
      struct variable *p = node->sons;
      int j = 0;
      bool flag = true;
      while (p) {
        if (j >= productions[i].num_sons || p->type != productions[i].sons[j]) {
          flag = false;
          break;
        }
        j++;
        p = p->brothers;
      }
      if (flag) {
        printf("Matched %d\n", i);
        struct variable *sons[5];
        struct variable *p = node->sons;
        int j = 0;
        while (p) {
          sons[j++] = p;
          if (p->sons) {
            translate(p);
          }
          p = p->brothers;
        }
        action(node, sons, i);
        return;
      }
    }
  }
  struct variable *p = node->sons;
  while (p) {
    printf("%d\n", p->type);
    p = p->brothers;
  }
  printf("\n");
  assert(0);
}

// 将c中的{{i}}替换为zipper[i]的值
char *backpatch(char *c) {
  char *code = (char *)malloc(10240);

  int i = 0, j = 0;
  while (c[i]) {
    if (c[i] == '{' && c[i + 1] == '{') {
      i += 2;
      int index = 0;
      while (c[i] != '}') {
        index = index * 10 + c[i] - '0';
        i++;
      }
      i += 2;
      printf("index: %d\n", index);
      printf("zipper[index]: %s\n", *zipper[index]);
      strcpy(code + j, *zipper[index]);
      j += strlen(*zipper[index]);
    } else {
      code[j++] = c[i++];
    }
  }

  code[j] = '\0';
  return code;
}

void do_translate(struct variable *root) {
  cal_type(root);
  translate(root);
  printf("%s", backpatch(root->info.code));
}