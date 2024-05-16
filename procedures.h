#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "tokens.h"
#include <stdio.h>

	struct variable
	{
		char name[7];
		struct variable* sons; //���������ͷ��㣬������
		struct variable* father;
		struct variable* brothers; //ָ����һ���ֵܽ���ָ��
	};



	//����ʶ���ȫ��token����������Ҫ������token��������token����������Ҫ����Ĳ���ʽ���飬���ر�����ʶ���״̬
	int A(struct token_t* tokens, int* index, int length, struct variable* A_node);
	int B(struct token_t* tokens, int* index, int length, struct variable* B_node);
	int C(struct token_t* tokens, int* index, int length, struct variable* C_node);
	int D(struct token_t* tokens, int* index, int length, struct variable* D_node);
	int E(struct token_t* tokens, int* index, int length, struct variable* E_node);
	int F(struct token_t* tokens, int* index, int length, struct variable* F_node);
	int H(struct token_t* tokens, int* index, int length, struct variable* H_node);
	int J(struct token_t* tokens, int* index, int length, struct variable* J_node);
	int P(struct token_t* tokens, int* index, int length, struct variable* P_node);
	int S(struct token_t* tokens, int* index, int length, struct variable* S_node);
	int T(struct token_t* tokens, int* index, int length, struct variable* T_node);


	void print_tree(struct variable* node); //��ӡ�﷨��

	void print_GraphTree(struct variable* node);
#ifdef __cplusplus
}
#endif