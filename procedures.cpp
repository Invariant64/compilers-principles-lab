#include "procedures.h"
#include <queue>

int A(struct token_t* tokens, int* index, int length, struct variable* A_node) {  //语法变量A的过程调用
	if (tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "IF") ||
		tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "WHILE")
		|| tokens[*index].type == TK_IDN) {//没有到达末尾//没有读到#
		struct variable* P_node = (struct variable*)malloc(sizeof(struct variable)); //孩子结点，这里是P
		strcpy(P_node->name, "P");
		P_node->father = A_node;//名字属性、父节点和兄弟结点在主调函数中设置
		P_node->brothers = NULL;
		P_node->sons = NULL;  //孙子结点默认为空

		A_node->sons = P_node; //自己的孩子链表的头结点在自己的调用过程赋值
		return P(tokens, index, length, P_node);//调用P过程
	}
	else if (*index >= length - 1) {
		struct variable* V_node = (struct variable*)malloc(sizeof(struct variable));
		strcpy(V_node->name, "ε");
		V_node->brothers = NULL;
		V_node->father = A_node;
		V_node->sons = NULL;
		A_node->sons = V_node;
		return 2; //读到结尾，使用空产生式，成功结束
	}
	else {
		A_node->sons = NULL;
		printf("A过程出错\n，出错index %d", *index);
		return 0;  //没有可以使用的产生式，失败
	}
}

int B(struct token_t* tokens, int* index, int length, struct variable* B_node) {
	if (tokens[*index].type == TK_ADD || tokens[*index].type == TK_SUB) {
		struct variable* add_sub = (struct variable*)malloc(sizeof(struct variable));
		struct variable* T_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* B_node2 = (struct variable*)malloc(sizeof(struct variable));
		//设置孩子的名字
		strcpy(add_sub->name, tokens[*index].value);
		strcpy(T_node->name, "T");
		strcpy(B_node2->name, "B");
		//在主调函数中赋值,孙子结点为NULL，
		add_sub->sons = NULL;
		T_node->sons = NULL;
		B_node->sons = NULL;
		//设置孩子的兄弟
		add_sub->brothers = T_node;
		T_node->brothers = B_node2;
		B_node2->brothers = NULL;
		//设置孩子的父亲
		add_sub->father = B_node;
		T_node->father = B_node;
		B_node2->father = B_node;
		//设置自己的长子
		B_node->sons = add_sub;

		(*index)++;
		int result = T(tokens, index, length, T_node);
		if (result == 0)
		{
			//T匹配失败
			return 0;  //失败
		}
		else if (result == 1) {
			(*index)++; //主调函数负责将正确的token索引传给被调函数
			return B(tokens, index, length, B_node2);
		}
		else if (result == 2) {
			return B(tokens, index, length, B_node2); //上一个返回的结果是空产生式，无需移动指针
		}
	}
	else if (tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "THEN") ||
		tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "DO") ||
		tokens[*index].type == TK_SRP || tokens[*index].type == TK_GT || tokens[*index].type == TK_LT ||
		tokens[*index].type == TK_EQ || tokens[*index].type == TK_SEM) {

		struct variable* V_node = (struct variable*)malloc(sizeof(struct variable));
		strcpy(V_node->name, "ε");
		V_node->brothers = NULL;
		V_node->father = B_node;
		V_node->sons = NULL;
		B_node->sons = V_node;
		return 2; //使用空产生式，成功返回
	}
	else {
		B_node->sons = NULL;
		printf("B过程出错\n");
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

		int result = E(tokens, index, length, E_node);

		if (result == 1) {
			(*index)++;
			return J(tokens, index, length, J_node);
		}
		else if (result == 2) {
			return J(tokens, index, length, J_node);
		}
		else if (result == 0) {
			return 0;
		}
	}
	else {
		C_node->sons = NULL;
		printf("C过程出错\n");
		return 0;
	}
}

int D(struct token_t* tokens, int* index, int length, struct variable* D_node) {
	if (tokens[*index].type == TK_MUL || tokens[*index].type == TK_DIV) {
		struct variable* mul_div = (struct variable*)malloc(sizeof(struct variable));
		struct variable* F_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* D_node2 = (struct variable*)malloc(sizeof(struct variable));
		//赋名
		strcpy(mul_div->name, tokens[*index].value);
		strcpy(F_node->name, "F");
		strcpy(D_node2->name, "D");
		//设定兄弟
		mul_div->brothers = F_node;
		F_node->brothers = D_node2;
		D_node2->brothers = NULL;
		//设定父亲
		mul_div->father = D_node;
		F_node->father = D_node;
		D_node2->father = D_node;
		//默认长子
		mul_div->sons = NULL;
		F_node->sons = NULL;
		D_node2->sons = NULL;
		//确定的长子
		D_node->sons = mul_div;

		(*index)++;
		int result = F(tokens, index, length, F_node);
		if (result == 1) {
			(*index)++;
			return D(tokens, index, length, D_node2);
		}
		else if (result == 2) {
			return D(tokens, index, length, D_node2);
		}
		else if (result == 0) {
			return 0;
		}
	}
	else if (tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "THEN") ||
		tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "DO") ||
		tokens[*index].type == TK_SRP || tokens[*index].type == TK_ADD || tokens[*index].type == TK_SUB ||
		tokens[*index].type == TK_GT || tokens[*index].type == TK_LT || tokens[*index].type == TK_EQ ||
		tokens[*index].type == TK_SEM) {

		struct variable* V_node = (struct variable*)malloc(sizeof(struct variable));
		strcpy(V_node->name, "ε");
		V_node->brothers = NULL;
		V_node->father = D_node;
		V_node->sons = NULL;
		D_node->sons = V_node;
		return 2;
	}
	else {
		D_node->sons = NULL;
		printf("D过程出错\n，出错index %d", *index);
		return 0;
	}
}

int E(struct token_t* tokens, int* index, int length, struct variable* E_node) {
	if (tokens[*index].type == TK_SLP || tokens[*index].type == TK_IDN || tokens[*index].type == TK_OCT || tokens[*index].type == TK_DEC ||
		tokens[*index].type == TK_HEX) {

		struct variable* T_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* B_node = (struct variable*)malloc(sizeof(struct variable));
		//赋名
		strcpy(T_node->name, "T");
		strcpy(B_node->name, "B");
		//设置兄弟
		T_node->brothers = B_node;
		B_node->brothers = NULL;
		//设置父亲
		T_node->father = E_node;
		B_node->father = E_node;
		//设置默认长子
		T_node->sons = NULL;
		B_node->sons = NULL;
		//设置确定长子
		E_node->sons = T_node;

		int result = T(tokens, index, length, T_node);
		if (result == 1) {
			(*index)++;
			return B(tokens, index, length, B_node);
		}
		else if (result == 2) {
			return B(tokens, index, length, B_node);
		}
		else if (result == 0) {
			return 0;
		}
	}
	else {
		E_node->sons = NULL;
		printf("E过程出错\n");
		return 0;
	}
}

int F(struct token_t* tokens, int* index, int length, struct variable* F_node) {
	if (tokens[*index].type == TK_SLP) {
		struct variable* left_br = (struct variable*)malloc(sizeof(struct variable));
		struct variable* E_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* right_br = (struct variable*)malloc(sizeof(struct variable));
		//赋名
		strcpy(left_br->name, "(");
		strcpy(E_node->name, "E");
		strcpy(right_br->name, ")");
		//设置兄弟
		left_br->brothers = E_node;
		E_node->brothers = right_br;
		right_br->brothers = NULL;
		//设置父亲
		left_br->father = F_node;
		E_node->father = F_node;
		right_br->father = F_node;
		//设置默认的长子
		left_br->sons = NULL;
		E_node->sons = NULL;
		right_br->sons = NULL;
		//设置确定的长子
		F_node->sons = left_br;

		(*index)++;
		int result = E(tokens, index, length, E_node);
		if (result == 1) {
			(*index)++;
			return tokens[*index].type == TK_SRP;
		}
		else if (result == 2) {
			return tokens[*index].type == TK_SRP;
		}
		else if (result == 0) {
			return 0;
		}
	}
	else if (tokens[*index].type == TK_IDN || tokens[*index].type == TK_OCT || tokens[*index].type == TK_DEC || tokens[*index].type == TK_HEX) {
		struct variable* tk = (struct variable*)malloc(sizeof(struct variable));
		//赋名
		strcpy(tk->name, tokens[*index].value);
		//设置兄弟、父亲、孩子
		tk->brothers = NULL;
		tk->father = F_node;
		tk->sons = NULL;
		F_node->sons = tk;
		return 1;
	}
	else {
		F_node->sons = NULL;
		printf("F过程出错\n");
		return 0;
	}
}

int H(struct token_t* tokens, int* index, int length, struct variable* H_node) {
	if (tokens[*index].type == TK_SEM) {
		struct variable* Sem_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* ELSE_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* S_node = (struct variable*)malloc(sizeof(struct variable));
		//赋名
		strcpy(Sem_node->name, ";");
		strcpy(ELSE_node->name, "ELSE");
		strcpy(S_node->name, "S");
		//设置兄弟
		Sem_node->brothers = ELSE_node;
		ELSE_node->brothers = S_node;
		S_node->brothers = NULL;
		//设置父亲
		Sem_node->father = H_node;
		ELSE_node->father = H_node;
		S_node->father = H_node;
		//设置默认长子
		Sem_node->sons = NULL;
		ELSE_node->sons = NULL;
		S_node->sons = NULL;
		//设置确定长子
		H_node->sons = Sem_node;

		(*index)++;
		int result = tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "ELSE");
		if (result == 1) {
			(*index)++;
			return S(tokens, index, length, S_node);
		}
		else { //回溯，使用空产生式
			free(ELSE_node);
			free(S_node);
			struct variable* V_node = (struct variable*)malloc(sizeof(struct variable));
			//赋名
			strcpy(V_node->name, "ε");
			//设置兄弟
			V_node->brothers = NULL;
			//设置父亲
			V_node->father = H_node;
			//设置默认长子
			V_node->sons = NULL;
			//设置确定长子
			H_node->sons = V_node;
			(*index)--; //未生成分号，回退一位
			return 2;
		}
	}
	else {
		H_node->sons = NULL;
		printf("H过程出错\n");
		return 0;
	}
}

int J(struct token_t* tokens, int* index, int length, struct variable* J_node) {
	if (tokens[*index].type == TK_GT || tokens[*index].type == TK_LT || tokens[*index].type == TK_EQ) {
		struct variable* cmp_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* E_node = (struct variable*)malloc(sizeof(struct variable));
		//赋名
		strcpy(cmp_node->name, tokens[*index].value);
		strcpy(E_node->name, "E");
		//设置兄弟
		cmp_node->brothers = E_node;
		E_node->brothers = NULL;
		//设置父亲
		cmp_node->father = J_node;
		E_node->father = J_node;
		//设置默认长子
		cmp_node->sons = NULL;
		E_node->sons = NULL;
		//设置确认长子
		J_node->sons = cmp_node;

		(*index)++;
		int result = E(tokens, index, length, E_node);
		if (result == 1) {
			return 1;
		}
		else if (result == 2) {
			return 2;
		}
		else if (result == 0) {
			return 0;
		}
	}
	else {
		J_node->sons = NULL;
		printf("J过程出错\n");
		return 0;
	}
}

int P(struct token_t* tokens, int* index, int length, struct variable* P_node) {
	if (tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "IF") ||
		tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "WHILE") ||
		tokens[*index].type == TK_IDN) {
		struct variable* S_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* Sem_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* A_node = (struct variable*)malloc(sizeof(struct variable));
		//赋名
		strcpy(S_node->name, "S");
		strcpy(Sem_node->name, ";");
		strcpy(A_node->name, "A");
		//设置兄弟
		S_node->brothers = Sem_node;
		Sem_node->brothers = A_node;
		A_node->brothers = NULL;
		//设置父亲
		S_node->father = P_node;
		Sem_node->father = P_node;
		A_node->father = P_node;
		//设置默认长子
		S_node->sons = NULL;
		Sem_node->sons = NULL;
		A_node->sons = NULL;
		//设置确定长子
		P_node->sons = S_node;

		//右部首元素是语法变量，不要移动index
		int result = S(tokens, index, length, S_node);
		if (result == 1) {
			(*index)++;
			if (tokens[*index].type == TK_SEM) {
				(*index)++;
				return A(tokens, index, length, A_node);
			}
			else {
				return 0;
			}
		}
		else if (result == 2) {
			if (tokens[*index].type == TK_SEM) {
				(*index)++;
				return A(tokens, index, length, A_node);
			}
			else {
				return 0;
			}
		}
		else if (result == 0) {
			return 0;
		}
	}
	else {
		P_node->sons = NULL;
		printf("P过程出错\n,出错token %s", tokens[*index].value);
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
		//赋名
		strcpy(IF_node->name, "IF");
		strcpy(C_node->name, "C");
		strcpy(THEN_node->name, "THEN");
		strcpy(S_node2->name, "S");
		strcpy(H_node->name, "H");
		//设置兄弟
		IF_node->brothers = C_node;
		C_node->brothers = THEN_node;
		THEN_node->brothers = S_node2;
		S_node2->brothers = H_node;
		H_node->brothers = NULL;
		//设置父亲
		IF_node->father = S_node;
		C_node->father = S_node;
		THEN_node->father = S_node;
		S_node2->father = S_node;
		H_node->father = S_node;
		//设置默认长子
		IF_node->sons = NULL;
		C_node->sons = NULL;
		THEN_node->sons = NULL;
		S_node2->sons = NULL;
		H_node->sons = NULL;
		//设置确定长子
		S_node->sons = IF_node;

		(*index)++;
		int result = C(tokens, index, length, C_node);
		if (result == 1) {
			(*index)++;
			result = (tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "THEN"));
			if (result == 1) {
				(*index)++;
				result = S(tokens, index, length, S_node2);
				if (result == 1) {
					(*index)++;
					return H(tokens, index, length, H_node);
				}
				else if (result == 2) {
					return H(tokens, index, length, H_node);
				}
				else if (result == 0) {
					return 0;
				}
			}
			else {
				printf("S过程出错，识别THEN出错，token index=%d", *index);
				return 0;
			}
		}
		else if (result == 2) {
			result = (tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "THEN"));
			if (result == 1) {
				(*index)++;
				result = S(tokens, index, length, S_node2);
				if (result == 1) {
					(*index)++;
					return H(tokens, index, length, H_node);
				}
				else if (result == 2) {
					return H(tokens, index, length, H_node);
				}
				else if (result == 0) {
					return 0;
				}
			}
			else {
				printf("S过程出错，识别THEN出错，token index=%d", *index);
				return 0;
			}
		}
		else if (result == 0) {
			return 0;
		}
	}
	else if (tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "WHILE")) {
		struct variable* WHILE_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* C_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* DO_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* S_node2 = (struct variable*)malloc(sizeof(struct variable));
		//赋名
		strcpy(WHILE_node->name, "WHILE");
		strcpy(C_node->name, "C");
		strcpy(DO_node->name, "DO");
		strcpy(S_node2->name, "S");
		//设置兄弟
		WHILE_node->brothers = C_node;
		C_node->brothers = DO_node;
		DO_node->brothers = S_node2;
		S_node2->brothers = NULL;
		//设置父亲
		WHILE_node->father = S_node;
		C_node->father = S_node;
		DO_node->father = S_node;
		S_node2->father = S_node;
		//设置默认长子
		WHILE_node->sons = NULL;
		C_node->sons = NULL;
		DO_node->sons = NULL;
		S_node2->sons = NULL;
		//设置确定长子
		S_node->sons = WHILE_node;

		(*index)++;
		int result = C(tokens, index, length, C_node);
		if (result == 1) {
			(*index)++;
			if (tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "DO")) {
				(*index)++;
				return S(tokens, index, length, S_node2);
			}
			else {
				printf("S过程出错，识别DO出错，token index=%d", *index);
				return 0;
			}
		}
		else if (result == 2) {
			if (tokens[*index].type == TK_KEY && !strcmp(keyword_type_str[atoi(tokens[*index].value)], "DO")) {
				(*index)++;
				return S(tokens, index, length, S_node2);
			}
			else {
				printf("S过程出错，识别DO出错，token index=%d", *index);
				return 0;
			}
		}
		else if (result == 0) {
			return 0;
		}
	}
	else if (tokens[*index].type == TK_IDN) {
		struct variable* id_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* EQ_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* E_node = (struct variable*)malloc(sizeof(struct variable));
		//赋名
		strcpy(id_node->name, tokens[*index].value);
		strcpy(EQ_node->name, "=");
		strcpy(E_node->name, "E");
		//设置兄弟
		id_node->brothers = EQ_node;
		EQ_node->brothers = E_node;
		E_node->brothers = NULL;
		//设置父亲
		id_node->father = S_node;
		EQ_node->father = S_node;
		E_node->father = S_node;
		//设置默认长子
		id_node->sons = NULL;
		EQ_node->sons = NULL;
		E_node->sons = NULL;
		//设置确认长子
		S_node->sons = id_node;

		(*index)++;
		if (tokens[*index].type == TK_EQ) {
			(*index)++;
			return E(tokens, index, length, E_node);
		}
		else {
			printf("S过程出错，识别=出错，token index=%d", *index);
			return 0;
		}
	}
	else {
		S_node->sons = NULL;
		printf("S过程出错\n, 出错的token %s", tokens[*index].value);
		return 0;
	}
}

int T(struct token_t* tokens, int* index, int length, struct variable* T_node) {
	if (tokens[*index].type == TK_IDN || tokens[*index].type == TK_OCT || tokens[*index].type == TK_DEC || tokens[*index].type == TK_HEX ||
		tokens[*index].type == TK_SLP) {
		struct variable* F_node = (struct variable*)malloc(sizeof(struct variable));
		struct variable* D_node = (struct variable*)malloc(sizeof(struct variable));
		//赋名
		strcpy(F_node->name, "F");
		strcpy(D_node->name, "D");
		//设置兄弟
		F_node->brothers = D_node;
		D_node->brothers = NULL;
		//设置父亲
		F_node->father = T_node;
		D_node->father = T_node;
		//设置默认长子
		F_node->sons = NULL;
		D_node->sons = NULL;
		//设置确认长子
		T_node->sons = F_node;

		int result = F(tokens, index, length, F_node);
		if (result == 1) {
			(*index)++;
			return D(tokens, index, length, D_node);
		}
		else if (result == 2) {
			return D(tokens, index, length, D_node);
		}
		else if (result == 0) {
			return 0;
		}
	}
	else {
		T_node->sons = NULL;
		printf("T过程出错\n");
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
			temp = q.front(); //队首结点
			son = temp->sons; //长子
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

void print_GraphTree(struct variable* node) {
	FILE* file = fopen("results/analysisTree.dot", "w");
	int letter[11] = { 0 }; //A B C D E F H J P S T 的个数
	if (file) {
		fprintf(file, "digraph G {\n");
	}
	else {
		printf("Failed to open file!\n");
	}

	std::queue<struct variable*> q;
	q.push(node);
	int num = 1; //记录数量
	if (file) {
		fprintf(file, "Node%d[label=\"%s\"];\n", num, node->name);
	}

	while (!q.empty()) {
		struct variable* temp = NULL;
		struct variable* son = NULL;
		struct variable* father = NULL;
		int size = q.size();
		while (size--)
		{
			temp = q.front(); //队首结点
			son = temp->sons; //长子
			while (son != NULL) {
				q.push(son);
				num++;
				fprintf(file, "Node%d[label=\"%s\"]; \n", num, son->name);
				son = son->brothers;
			}
			q.pop();
		}
	}

	std::queue<int> p;

	q.push(node);
	int num2 = 1; //记录数量
	p.push(num2);
	int father_index = 0;
	while (!q.empty()) {
		struct variable* temp = NULL;
		struct variable* son = NULL;
		struct variable* father = NULL;
		int size = q.size();
		while (size--)
		{
			temp = q.front(); //队首结点
			son = temp->sons; //长子
			father_index = p.front();
			p.pop();
			while (son != NULL) {
				q.push(son);
				num2++;
				p.push(num2);
				fprintf(file, "Node%d -> Node%d;\n", father_index, num2);
				son = son->brothers;
			}
			q.pop();
		}
	}

	fprintf(file, "}\n");
	fclose(file);
}