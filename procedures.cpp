#include "procedures.h"
#include <queue>

int A(struct token_t* tokens, int* index,int length, struct variable* A_node) {  //�﷨����A�Ĺ��̵���
	if (tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "IF") ||
		tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "WHILE")
		|| tokens[*index].type == TK_IDN) {//û�е���ĩβ//û�ж���#
		struct variable* P_node= (struct variable*)malloc(sizeof(struct variable)); //���ӽ�㣬������P
		strcpy(P_node->name, "P");
		P_node->father = A_node;//�������ԡ����ڵ���ֵܽ������������������
		P_node->brothers = NULL;
		P_node->sons = NULL;  //���ӽ��Ĭ��Ϊ��
		
		A_node->sons = P_node; //�Լ��ĺ��������ͷ������Լ��ĵ��ù��̸�ֵ
		return P(tokens, index, length,P_node);//����P����
	}
	else if(*index>=length-1){
		struct variable* V_node = (struct variable*)malloc(sizeof(struct variable));
		strcpy(V_node->name, "��");
		V_node->brothers = NULL;
		V_node->father = A_node;
		V_node->sons = NULL;
		A_node->sons = V_node;
		(*index)--;
		return 1; //������β��ʹ�ÿղ���ʽ���ɹ�����
	}
	else {
		A_node->sons = NULL;
		printf("A���̳���\n");
		return 0;  //û�п���ʹ�õĲ���ʽ��ʧ��
	}
}

int B(struct token_t* tokens, int* index, int length, struct variable* B_node) {
	if (tokens[*index].type== TK_ADD || tokens[*index].type == TK_SUB) {
		struct variable* add_sub = (struct variable*)malloc(sizeof(struct variable));
		struct variable* T_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* B_node2 = (struct variable*)malloc(sizeof(struct variable));
		//���ú��ӵ�����
		strcpy(add_sub->name,tokens[*index].value);
		strcpy(T_node->name, "T");
		strcpy(B_node2->name, "B");
		//�����������и�ֵ,���ӽ��ΪNULL��
		add_sub->sons = NULL;
		T_node->sons = NULL;
		B_node->sons = NULL;
		//���ú��ӵ��ֵ�
		add_sub->brothers = T_node;
		T_node->brothers = B_node2;
		B_node2->brothers = NULL;
		//���ú��ӵĸ���
		add_sub->father = B_node;
		T_node->father = B_node;
		B_node2->father = B_node;
		//�����Լ��ĳ���
		B_node->sons = add_sub;

		(*index)++;
		if (T(tokens, index, length,T_node) == 1) {
			(*index)++; //��������������ȷ��token����������������
			return B(tokens, index, length,B_node2);
		}
		else
		{
			//Tƥ��ʧ��
			return 0;  //ʧ��
		}
	}
	else if (tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "ELSE") ||
			 tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "THEN") ||
			 tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "DO") ||
			 tokens[*index].type==TK_SRP|| tokens[*index].type==TK_GT|| tokens[*index].type == TK_LT||
			 tokens[*index].type == TK_EQ || *index >= length - 1){

		struct variable* V_node = (struct variable*)malloc(sizeof(struct variable));
		strcpy(V_node->name, "��");
		V_node->brothers = NULL;
		V_node->father = B_node;
		V_node->sons = NULL;
		B_node->sons = V_node;
		(*index)--;
		return 1; //ʹ�ÿղ���ʽ���ɹ�����
	}
	else {
		B_node->sons = NULL;
		printf("B���̳���\n");
		return 0;
	}
}

int C(struct token_t* tokens, int* index, int length, struct variable* C_node) {
	if (tokens[*index].type == TK_SLP || tokens[*index].type == TK_IDN || tokens[*index].type == TK_OCT ||
		tokens[*index].type == TK_DEC || tokens[*index].type == TK_HEX) {
		struct variable* E_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* J_node = (struct variable*)malloc(sizeof(struct variable));

		strcpy(E_node->name, "E");
		strcpy(J_node->name, "J");

		E_node->brothers = J_node;
		J_node->brothers = NULL;

		E_node->father = C_node;
		J_node->father = C_node;

		E_node->sons = NULL;
		J_node->sons = NULL;

		C_node->sons = E_node;

		if (E(tokens, index, length, E_node) == 1) {
			(*index)++;
			return J(tokens, index, length, J_node);
		}
		else {
			return 0;
		}
	}
	else {
		C_node->sons = NULL;
		printf("C���̳���\n");
		return 0;
	}
}

int D(struct token_t* tokens, int* index, int length, struct variable* D_node) {
	if (tokens->type == TK_MUL || tokens->type == TK_DIV) {
		struct variable* mul_div = (struct variable*)malloc(sizeof(struct variable));
		struct variable* F_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* D_node2 = (struct variable*)malloc(sizeof(struct variable));
		//����
		strcpy(mul_div->name, tokens[*index].value);
		strcpy(F_node->name, "F");
		strcpy(D_node2->name, "D");
		//�趨�ֵ�
		mul_div->brothers = F_node;
		F_node->brothers = D_node2;
		D_node2->brothers = NULL;
		//�趨����
		mul_div->father = D_node;
		F_node->father = D_node;
		D_node2->father = D_node;
		//Ĭ�ϳ���
		mul_div->sons = NULL;
		F_node->sons = NULL;
		D_node2->sons = NULL;
		//ȷ���ĳ���
		D_node->sons = mul_div;

		(*index)++;
		if (F(tokens, index, length, F_node) == 1) {
			(*index)++;
			return D(tokens, index, length, D_node2);
		}
		else {
			return 0;
		}
	}
	else if (tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "ELSE") || 
			 tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "THEN") || 
			 tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "DO") ||
			 tokens[*index].type == TK_SRP || tokens[*index].type == TK_ADD || tokens[*index].type == TK_SUB||
			 tokens[*index].type == TK_GT || tokens[*index].type == TK_LT || tokens[*index].type == TK_EQ
			 || *index >= length - 1) {

		struct variable* V_node = (struct variable*)malloc(sizeof(struct variable));
		strcpy(V_node->name, "��");
		V_node->brothers = NULL;
		V_node->father = D_node;
		V_node->sons = NULL;
		D_node->sons = V_node;
		(*index)--;
		return 1;
	}
	else {
		D_node->sons = NULL;
		printf("D���̳���\n������index %d",*index);
		return 0;
	}
}

int E(struct token_t* tokens, int* index, int length, struct variable* E_node) {
	if (tokens[*index].type == TK_SLP || tokens[*index].type == TK_IDN || tokens[*index].type == TK_OCT || tokens[*index].type == TK_DEC ||
		tokens[*index].type == TK_HEX) {

		struct variable* T_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* B_node = (struct variable*)malloc(sizeof(struct variable));
		//����
		strcpy(T_node->name, "T");
		strcpy(B_node->name, "B");
		//�����ֵ�
		T_node->brothers = B_node;
		B_node->brothers = NULL;
		//���ø���
		T_node->father = E_node;
		B_node->father = E_node;
		//����Ĭ�ϳ���
		T_node->sons = NULL;
		B_node->sons = NULL;
		//����ȷ������
		E_node->sons = T_node;

		if (T(tokens, index, length, T_node) == 1) {
			(*index)++;
			return B(tokens, index, length, B_node);
		}
		else {
			return 0;
		}
	}
	else {
		E_node->sons = NULL;
		printf("E���̳���\n");
		return 0;
	}
}

int F(struct token_t* tokens, int* index, int length, struct variable* F_node) {
	if (tokens[*index].type == TK_SLP) {
		struct variable* left_br = (struct variable*)malloc(sizeof(struct variable));
		struct variable* E_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* right_br = (struct variable*)malloc(sizeof(struct variable));
		//����
		strcpy(left_br->name, "(");
		strcpy(E_node->name, "E");
		strcpy(right_br->name, ")");
		//�����ֵ�
		left_br->brothers = E_node;
		E_node->brothers = right_br;
		right_br->brothers = NULL;
		//���ø���
		left_br->father = F_node;
		E_node->father = F_node;
		right_br->father = F_node;
		//����Ĭ�ϵĳ���
		left_br->sons = NULL;
		E_node->sons = NULL;
		right_br->sons = NULL;
		//����ȷ���ĳ���
		F_node->sons = left_br;

		(*index)++;
		if (E(tokens, index, length, E_node) == 1) {
			(*index)++;
			return tokens[*index].type == TK_SRP;
		}
		else {
			return 0;
		}
	}
	else if (tokens[*index].type == TK_IDN || tokens[*index].type == TK_OCT || tokens[*index].type == TK_DEC || tokens[*index].type == TK_HEX) {
		struct variable* tk = (struct variable*)malloc(sizeof(struct variable));
		//����
		strcpy(tk->name, tokens[*index].value);
		//�����ֵܡ����ס�����
		tk->brothers = NULL;
		tk->father = F_node;
		tk->sons = NULL;
		F_node->sons = tk;
		return 1;
	}
	else {
		F_node->sons = NULL;
		printf("F���̳���\n");
		return 0;
	}
}

int H(struct token_t* tokens, int* index, int length, struct variable* H_node) {
	if (tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "ELSE")) {
		struct variable* ELSE_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* S_node = (struct variable*)malloc(sizeof(struct variable));
		//����
		strcpy(ELSE_node->name, "ELSE");
		strcpy(S_node->name, "S");
		//�����ֵ�
		ELSE_node->brothers = S_node;
		S_node->brothers = NULL;
		//���ø���
		ELSE_node->father = H_node;
		S_node->father = H_node;
		//����Ĭ�ϳ���
		ELSE_node->sons = NULL;
		S_node->sons = NULL;
		//����ȷ������
		H_node->sons = ELSE_node;

		(*index)++;
		if (S(tokens, index, length, S_node) == 1) {
			return 1;
		}
		else { //���ݣ�ʹ�ÿղ���ʽ
			strcpy(H_node->name, "��");
			H_node->sons = NULL;
			free(ELSE_node);
			free(S_node);
			return 1;
		}
	}
	else if (*index >= length - 1) {
		struct variable* V_node = (struct variable*)malloc(sizeof(struct variable));
		strcpy(V_node->name, "��");
		V_node->brothers = NULL;
		V_node->father = H_node;
		V_node->sons = NULL;
		H_node->sons = V_node;
		(*index)--;
		return 1;
	}
	else {
		H_node->sons = NULL;
		printf("H���̳���\n");
		return 0;
	}
}

int J(struct token_t* tokens, int* index, int length, struct variable* J_node) {
	if (tokens[*index].type == TK_GT || tokens[*index].type == TK_LT || tokens[*index].type == TK_EQ) {
		struct variable* cmp_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* E_node = (struct variable*)malloc(sizeof(struct variable));
		//����
		strcpy(cmp_node->name, tokens[*index].value);
		strcpy(E_node->name, "E");
		//�����ֵ�
		cmp_node->brothers = E_node;
		E_node->brothers = NULL;
		//���ø���
		cmp_node->father = J_node;
		E_node->father = J_node;
		//����Ĭ�ϳ���
		cmp_node->sons = NULL;
		E_node->sons = NULL;
		//����ȷ�ϳ���
		J_node->sons = cmp_node;

		(*index)++;
		if (E(tokens, index, length, E_node) == 1) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else {
		J_node->sons = NULL;
		printf("J���̳���\n");
		return 0;
	}
}

int P(struct token_t* tokens, int* index, int length, struct variable* P_node) {
	if (tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "IF") ||
		tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "WHILE") || 
		tokens[*index].type == TK_IDN) {
		struct variable* S_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* A_node = (struct variable*)malloc(sizeof(struct variable));
		//����
		strcpy(S_node->name, "S");
		strcpy(A_node->name, "A");
		//�����ֵ�
		S_node->brothers = A_node;
		A_node->brothers = NULL;
		//���ø���
		S_node->father = P_node;
		A_node->father = P_node;
		//����Ĭ�ϳ���
		S_node->sons = NULL;
		A_node->sons = NULL;
		//����ȷ������
		P_node->sons = S_node;

		//�Ҳ���Ԫ�����﷨��������Ҫ�ƶ�index
		if (S(tokens, index, length, S_node) == 1) {
			return A(tokens, index, length, A_node);
		}
		else {
			return 0;
		}
	}
	else {
		P_node->sons = NULL;
		printf("P���̳���\n,����token %s",tokens[*index].value);
		return 0;
	}
}

int S(struct token_t* tokens, int* index, int length, struct variable* S_node) {
	if (tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "IF")) {
		struct variable* IF_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* C_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* THEN_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* S_node2 = (struct variable*)malloc(sizeof(struct variable));
		struct variable* H_node = (struct variable*)malloc(sizeof(struct variable));
		//����
		strcpy(IF_node->name, "IF");
		strcpy(C_node->name, "C");
		strcpy(THEN_node->name, "THEN");
		strcpy(S_node2->name, "S");
		strcpy(H_node->name, "H");
		//�����ֵ�
		IF_node->brothers = C_node;
		C_node->brothers = THEN_node;
		THEN_node->brothers = S_node2;
		S_node2->brothers = H_node;
		H_node->brothers = NULL;
		//���ø���
		IF_node->father = S_node;
		C_node->father = S_node;
		THEN_node->father = S_node;
		S_node2->father = S_node;
		H_node->father = S_node;
		//����Ĭ�ϳ���
		IF_node->sons = NULL;
		C_node->sons = NULL;
		THEN_node->sons = NULL;
		S_node2->sons = NULL;
		H_node->sons = NULL;
		//����ȷ������
		S_node->sons = IF_node;

		(*index)++;
		if (C(tokens, index, length, C_node) == 1) {
			(*index)++;
			if (tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "THEN")) {
				(*index)++;
				if (S(tokens, index, length, S_node2) == 1) {
					(*index)++;
					return H(tokens, index, length, H_node);
				}
				else {
					return 0;
				}
			}
			else {
				return 0;
			}
		}
		else {
			return 0;
		}
	}
	else if (tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "WHILE")) {
		struct variable* WHILE_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* C_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* DO_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* S_node2 = (struct variable*)malloc(sizeof(struct variable));
		//����
		strcpy(WHILE_node->name, "WHILE");
		strcpy(C_node->name, "C");
		strcpy(DO_node->name, "DO");
		strcpy(S_node2->name, "S");
		//�����ֵ�
		WHILE_node->brothers = C_node;
		C_node->brothers = DO_node;
		DO_node->brothers = S_node2;
		S_node2->brothers = NULL;
		//���ø���
		WHILE_node->father = S_node;
		C_node->father = S_node;
		DO_node->father = S_node;
		S_node2->father = S_node;
		//����Ĭ�ϳ���
		WHILE_node->sons = NULL;
		C_node->sons = NULL;
		DO_node->sons = NULL;
		S_node2->sons = NULL;
		//����ȷ������
		S_node->sons = WHILE_node;

		(*index)++;
		if (C(tokens, index, length, C_node) == 1) {
			(*index)++;
			if (tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "DO")) {
				(*index)++;
				return S(tokens, index, length, S_node2);
			}
			else {
				return 0;
			}
		}
		else {
			return 0;
		}
	}
	else if (tokens[*index].type == TK_IDN) {
		struct variable* id_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* EQ_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* E_node = (struct variable*)malloc(sizeof(struct variable));
		//����
		strcpy(id_node->name, tokens[*index].value);
		strcpy(EQ_node->name, "=");
		strcpy(E_node->name, "E");
		//�����ֵ�
		id_node->brothers = EQ_node;
		EQ_node->brothers = E_node;
		E_node->brothers = NULL;
		//���ø���
		id_node->father = S_node;
		EQ_node->father = S_node;
		E_node->father = S_node;
		//����Ĭ�ϳ���
		id_node->sons = NULL;
		EQ_node->sons = NULL;
		E_node->sons = NULL;
		//����ȷ�ϳ���
		S_node->sons = id_node;

		(*index)++;
		if (tokens[*index].type==TK_EQ) {
			(*index)++;
			return E(tokens, index, length, E_node);
		}
		else {
			return 0;
		}
	}
	else {
		S_node->sons = NULL;
		printf("S���̳���\n, �����token %s",tokens[*index].value);
		return 0;
	}
}

int T(struct token_t* tokens, int* index, int length, struct variable* T_node) {
	if (tokens[*index].type == TK_IDN || tokens[*index].type == TK_OCT || tokens[*index].type == TK_DEC || tokens[*index].type == TK_HEX ||
		tokens[*index].type == TK_SLP) {
		struct variable* F_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* D_node = (struct variable*)malloc(sizeof(struct variable));
		//����
		strcpy(F_node->name, "F");
		strcpy(D_node->name, "D");
		//�����ֵ�
		F_node->brothers = D_node;
		D_node->brothers = NULL;
		//���ø���
		F_node->father = T_node;
		D_node->father = T_node;
		//����Ĭ�ϳ���
		F_node->sons = NULL;
		D_node->sons = NULL;
		//����ȷ�ϳ���
		T_node->sons = F_node;
		
		if (F(tokens, index, length, F_node) == 1) {
			(*index)++;
			return D(tokens, index, length, D_node);
		}
		else {
			return 0;
		}
	}
	else {
		T_node->sons = NULL;
		printf("T���̳���\n");
		return 0;
	}
}

void print_tree(struct variable* node) {
	std::queue<struct variable*> q;
	q.push(node);

	while (!q.empty()) {
		struct variable* temp = NULL;
		struct variable* son = NULL;
		int size = q.size();
		while (size--)
		{
			temp = q.front(); //���׽��
			son = temp->sons; //����
			while (son != NULL) {
				q.push(son);
				son = son->brothers;
			}
			printf("%s ", q.front()->name);
			if (q.front()->brothers == NULL) {
				printf("|");
			}
			q.pop();
		}
		printf("\n");
		
	}
}