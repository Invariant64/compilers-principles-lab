#include<iostream>
#include <algorithm>
#include<fstream>
#include<vector>
#include<string>
#include<map>
#include<set>
#include<cstring>
#include<unordered_map>
#include <filesystem>
#include "lex.h"
#include "tokens.c"

using namespace std;

vector< vector<char> > G; 

unordered_map<char, set<char> > ts; 

unordered_map<char, set<char> > nts; 

map< map<string, char>, string> table; 

struct token_t tokens[1024];
int nr_tokens = 0;

#define DEFAULT_INPUT "IF 5 > 3 THEN a = b ELSE IF 3 > 2 THEN b = a; c = d;"

void read_G();       
void get_First();    
void get_Closure(); 
int get_Table();     
int check(int time,int start_idx,int end_idx); 
void show_G();       
void show_Symbol();  
void show_First();   
void show_Closure(); 
void show_Table();   


struct C { //�հ�CLOSURE
	
	vector< vector<char> > project; //��Ŀ��
	
	vector< set<char> > outlook; //չ����
	
	unordered_map<char, int> go; //GO����
};
vector<C> c;

//����ع��ķ�

void show_G() {
	
	cout << "ԭ�ķ��ع�Ϊ�ķ�G[M]��" << endl;
	
	for (unsigned int i = 0; i < G.size(); i++) { 
		
		cout << i << "��";
		
		for (unsigned int j = 0; j < G[i].size(); j++) {
			
			if (j == 1)cout << "->";
			if (G[i][j] - 48 >= 1 && G[i][j] - 48 <= 4)
				cout << token_type_str[G[i][j] - 48];
			else cout << G[i][j];
		}
		cout << endl;
	}
	cout << endl << endl;
}


//������ս�����ս��

void show_Symbol() {
	
	for (unordered_map<char, set<char> >::iterator it = nts.begin(); it != nts.end(); it++) { 
	
		cout << it->first;
	}
	
	cout << endl;
	
	for (unordered_map<char, set<char> >::iterator it = ts.begin(); it != ts.end(); it++) { 
		cout << it->first;
	}
	cout << endl << endl;
}


//���First��

void show_First() {
	
	for (auto it1 : ts) {
		
		cout << it1.first << ": ";
		
		for (auto it2 : it1.second) {
			cout << it2 << ",";
		}
		cout << endl;
	}
	
	cout << endl << endl;
	for (auto it1 : nts) {
		cout << it1.first << ": ";
		for (auto it2 : it1.second) {
			cout << it2 << ",";
		}
		cout << endl;
	}
	cout << endl << endl;
}



//����հ�

void show_Closure() {  //��Ŀ����GO���������Closure.txt�ļ�
	fstream f("Closure.txt", ios::out);
	if (!f) {
		cout << "Closure.txt�ļ��򿪳���" << endl;
		return;
	}
	f << "���ķ�����Ŀ����GO������" << endl;
	for (unsigned int i = 0; i < c.size(); i++) {
		f << "I" << i << ":" << endl;
		for (unsigned int j = 0; j < c[i].project.size(); j++) {
			for (unsigned int k = 0; k < c[i].project[j].size(); k++) {
				if (k == 1) f << "->";
				if (c[i].project[j][k] == ' ') f << "��";
				else f << c[i].project[j][k];
			}
			f << "��";
			for (auto it : c[i].outlook[j]) {
				if (it == *(c[i].outlook[j].begin())) f << it;
				else f << "/" << it;
			}
			f << endl;
		}
		for (auto it : c[i].go) {
			f << "GO(I" << i << "��" << it.first << ") = I" << it.second << endl;
		}
		f << endl;
	}
	cout << "�ѽ���Ŀ����GO�������ɵ�Closure.txt�ļ��С�" << endl << endl;
}



//���LR������

void show_Table() {  //LR�����������LR_Table.txt�ļ�
	
	fstream f("LR_Table.txt", ios::out);
	
	if (!f) {
		cout << "LR_Table.txt�ļ��򿪳���" << endl;
		return;
	}
	
	for (int i = -1; i < (int)c.size(); i++) {
		
		if (i == -1) f << " " << '\t';
		
		else f << i << '\t';
		
		for (auto it : ts) {
			if (i == -1) {
				f << it.first << '\t';
			}
			else {
				map<string, char> m; 
				m[to_string(i)] = it.first;
				f << table[m] << '\t';
			}
		}
		if (i == -1) f << "#" << '\t';
		else {
			map<string, char> m;
			m[to_string(i)] = '#';
			f << table[m] << '\t';
		}
		for (auto it : nts) {
			if (it.first == 'M') continue;
			if (i == -1)f << it.first << '\t';
			else {
				map<string, char> m;
				m[to_string(i)] = it.first;
				f << table[m] << '\t';
			}
		}
		f << endl;
	}
	f.close();
	cout << "�ѽ�LR���������ɵ�LR_Table.txt�ļ��С�" << endl << endl;
	/*for (auto it1 : table) {
		for (auto it2 : it1.first) {
			cout << it2.first << it2.second << ":"<<it1.second<<endl;
		}
	}*/
}



//��ȡ�ķ�G[S]->G'[M]���������ս���ͷ��ս�� 

void read_G() { 
	char ch; 
	int i = 0; 
	vector<char> v; 
	char X; 
	set<char> m;
	nts['M'] = m;
	FILE* fp = fopen("production.txt", "r");
	if (fp == NULL) {
		cout << "�޷����ļ�production.txt" << endl;
		exit(0);
	}

	while (ch = fgetc(fp)) {
		if (ch == '#') break;
		if (ch == '\n') { 
			if (!v.empty()) G.push_back(v);
			v.clear();
			i = 0;
			continue;
		}
		if (ch != ' ') { 
			if (ch == '|') { 
				G.push_back(v);
				v.clear();
				i = 3;
				v.push_back(X);
				continue;
			}
			i++;
			if (i == 1) {
				X = ch;
				nts[ch] = m; 
			}
			else if (i != 2 && i != 3 && ch != '$') ts[ch] = m; 
			if (i != 2 && i != 3)v.push_back(ch); 
		}
	}
	fclose(fp);
	if (G.empty()) exit(0);

	
	v.clear();
	v.push_back('M');
	v.push_back(G[0][0]);
	G.insert(G.begin(), v);
	
	
	for (unordered_map<char, set<char> >::iterator it = nts.begin(); it != nts.end(); it++) {
		unordered_map<char, set<char> >::iterator iter;
		iter = ts.find(it->first);
		if (iter != ts.end())ts.erase(iter);
	}
}



//����First����

void get_First() { 
	
	for (auto& it : ts) it.second.insert(it.first);

	
	
	int r = 0;
	
	int change = 1;
	
	while (change) {
		
		if (r == 20)break;
		
		r++;
		
		change = 0;
		
		for (auto& it : nts) { //��ÿ�����ս��
			for (unsigned int i = 0; i < G.size(); i++) { //��������ʽ
				if (G[i][0] == it.first) {
					unsigned int size = it.second.size(); //����ǰFirst(X)�Ĵ�С
					unordered_map<char, set<char> >::iterator iter = ts.find(G[i][1]);
					if (ts.find(G[i][1]) != ts.end() || G[i][1] == '$') { 
						it.second.insert(G[i][1]);
						if (it.second.size() > size) change = 1;
					}
					else {  
						unsigned int col = 1;
						while (1) { 
							int flag = 0; 
							unordered_map<char, set<char> >::iterator itt = nts.find(G[i][col]);
							for (auto& iter : itt->second) { 
								if (iter == '$') flag = 1;
								else it.second.insert(iter);
							}
							if (flag) {
								col++;
								if (G[i].size() <= col) {
									it.second.insert('$'); 
									break;
								}
								else if (ts.find(G[i][col]) != ts.end()) { 
									it.second.insert(G[i][col]);
									break;
								}
								else { 
								}
							}
							else break;
						}
						if (it.second.size() > size) change = 1;
					}
				}
			}
		}
	}
}



//����CLOSURE������GO

void get_Closure() {  
	
	int i = 0; //�հ����
	
	C clo; //���ɵ�һ���հ���I0��
	
	c.push_back(clo);
	
	while (1) {
		
		if (i == c.size()) break; 
		
		if (i == 0) { 
			vector<char> v(G[0]);
			v.insert(v.begin() + 1, ' ');
			c[i].project.push_back(v);
			set<char> m;
			m.insert('#');
			c[i].outlook.push_back(m);
		}
		for (unsigned int j = 0; j < c[i].project.size(); j++) {  
			
			for (unsigned int k = 0; k < c[i].project[j].size(); k++) {  
				
				if (c[i].project[j][k] == ' ') {
					
					if (k == c[i].project[j].size() - 1) break;  
					
					for (unsigned int x = 0; x < G.size(); x++) { 
						
						if (G[x][0] == c[i].project[j][k + 1]) { 
							vector<char> v(G[x]);
							v.insert(v.begin() + 1, ' '); 
							int exist = 0; 
							for (unsigned int y = 0; y < c[i].project.size(); y++) { 
								if (c[i].project[y] == v) { 
									exist = y;
									break;
								}
							}
							if (exist == 0) c[i].project.push_back(v); 
							set<char> m; 
							bool kong = true; 
							int t = 0; 
							while (kong) { 
								kong = false;
								if (k + t + 1 == c[i].project[j].size() - 1) {  //���һ
									for (auto it : c[i].outlook[j]) m.insert(it);
								}
								else if (ts.find(c[i].project[j][k + t + 2]) != ts.end()) {  //�����
									m.insert(c[i].project[j][k + 2 + t]);
								}
								else {  //�����
									set<char> m1((nts.find(c[i].project[j][k + 2 + t]))->second);
									for (auto it : m1) {
										if (it == '$') {  //������
											kong = true;
											t++;
										}
										else { //��������
											m.insert(it);
										}
									}
								}
							}
							if (exist) 
							{
								for (auto it : m) {
									c[i].outlook[exist].insert(it);
								}
							}
							else c[i].outlook.push_back(m); 
						}
					}
					break;
				}
			}
		}

		for (unsigned int j = 0; j < c[i].project.size(); j++) {  
			for (unsigned int k = 0; k < c[i].project[j].size(); k++) { 
				if (c[i].project[j][k] == ' ') {
					if (k == c[i].project[j].size() - 1) break; 
					
					vector<char> new_closure_pro(c[i].project[j]); 
					new_closure_pro[k] = new_closure_pro[k + 1]; 
					new_closure_pro[k + 1] = ' ';
					set<char> new_closure_search(c[i].outlook[j]); 
					bool dif = false; 
					for (unsigned int x = 0; x < c.size(); x++) { 
						//dif = false;
						for (unsigned int y = 0; y < c[x].project.size(); y++) { 
							dif = false;
							if (new_closure_pro == c[x].project[y]) {
								if (c[x].outlook[0].size() != new_closure_search.size()) {
									dif = true;
									continue;
								}
								auto iter = c[x].outlook[0].begin();
								for (auto it : new_closure_search) {  
									if (it != *iter) {
										dif = true;
										break;
									}
									iter++;
								}
								if (dif == false) { 
									c[i].go[new_closure_pro[k]] = x;
									break;
								}
							}
							else dif = true;
							if (dif == false) break;
						}
						if (dif == false) break;
					}
					if (c[i].go.count(new_closure_pro[k]) != 0 && dif) { 
						c[c[i].go[new_closure_pro[k]]].project.push_back(new_closure_pro); 
						c[c[i].go[new_closure_pro[k]]].outlook.push_back(new_closure_search);
						break;
					}
					if (dif) { 
						C new_closure;
						new_closure.project.push_back(new_closure_pro);
						new_closure.outlook.push_back(new_closure_search);
						c.push_back(new_closure);
						c[i].go[new_closure_pro[k]] = c.size() - 1;
					}
				}
			}
		}
		i++; //��һ�հ�
	}
}



//��CLOSURE����LR(1)������table

int get_Table() { 
	
	for (unsigned int i = 0; i < c.size(); i++) { //�������бհ�
		
		for (unsigned int j = 0; j < c[i].project.size(); j++) { //����ÿ���հ��е�������Ŀ
			
			for (unsigned int k = 0; k < c[i].project[j].size(); k++) { //ɨ�����Ŀ,�ҵ���ǰλ��
				
				if (c[i].project[j][k] == ' ') {
					//cout << 1 << endl;
					/*
					if (k == 0 && c[i].project[j][k + 1] == '$') { //���� ��X->�����¡�����Լ�մ�
						int id;
						for (unsigned int x = 0; x < G.size(); x++) { //ɨ��G'[M]�ҵ���Ӧ�Ĳ���ʽ�ı��
							vector<char> v(c[i].project[j]);
							v.pop_back();
							if (G[x] == v) {
								id = x;
								break;
							}
						}
						for (auto it : c[i].outlook[j]) {
							map<string, char> m;
							m[to_string(i)] = it;
							if (table.find(m) != table.end() && table[m] != (string)"r" + to_string(id)) {
								cout << "���ķ�����LR(1)�ķ������ڶ��ض�����ڣ�" << endl;
								return 0;
							}
							else table[m] = (string)"r" + to_string(id);
						}
					}
					else if (k == c[i].project[j].size() - 1 && c[i].project[j][k - 1] == '$') { //���� ��X->$�����¡�����Լ�մ�
                        int id;
						for (unsigned int x = 0; x < G.size(); x++) { //ɨ��G'[M]�ҵ���Ӧ�Ĳ���ʽ�ı��
							vector<char> v(c[i].project[j]);
							v.pop_back();
							if (G[x] == v) {
								id = x;
								break;
							}
						}
						for (auto it : c[i].outlook[j]) {
							map<string, char> m;
							m[to_string(i)] = it;
							if (table.find(m) != table.end() && table[m] != (string)"r" + to_string(id)) {
								cout << "���ķ�����LR(1)�ķ������ڶ��ض�����ڣ�" << endl;
								return 0;
							}
							else table[m] = (string)"r" + to_string(id);
						}
                    }*/
					if (k == c[i].project[j].size() - 1) { //���� ��X->�������¡�����Լ/acc
						
						if (c[i].project[j][0] == 'M') {  //���� ��M->X����#������table[i,#]=acc
							map<string, char> m;
							m[to_string(i)] = '#';
							
							if (table.find(m) != table.end() && table[m] != "acc") {
								cout << "���ķ�����LR(1)�ķ������ڶ��ض�����ڣ�" << endl;
								return 0;
							}
							else table[m] = "acc";
						}
						else { //���� ��X->������a������Լ����table[i,a]=rj
							int id;
							for (unsigned int x = 0; x < G.size(); x++) { //ɨ��G'[M]�ҵ���Ӧ�Ĳ���ʽ�ı��
								vector<char> v(c[i].project[j]);
								v.pop_back();
								if (G[x] == v) {
									id = x;
									break;
								}
							}
							for (auto it : c[i].outlook[j]) {
								map<string, char> m;
								m[to_string(i)] = it;
								if (table.find(m) != table.end() && table[m] != (string)"r" + to_string(id)) {
									cout << "���ķ�����LR(1)�ķ������ڶ��ض�����ڣ�" << endl;
									return 0;
								}
								else table[m] = (string)"r" + to_string(id);
							}
						}
					}
					else { //���� ��X->�����£��á�
						char next = c[i].project[j][k + 1];
						if (ts.find(next) != ts.end()) {  //���� ��X->����a�£��á�,��table[i,a]=sj
							map<string, char> m;
							m[to_string(i)] = next;
							if (table.find(m) != table.end() && table[m] != (string)"s" + to_string(c[i].go[next])) {
								cout << "���ķ�����LR(1)�ķ������ڶ��ض�����ڣ�" << endl;
								return 0;
							}
							else table[m] = (string)"s" + to_string(c[i].go[next]);
						}
						else { //���� ��X->����Y�£��á�,��table[i,Y]=j
							map<string, char> m;
							m[to_string(i)] = next;
							if (table.find(m) != table.end() && table[m] != to_string(c[i].go[next])) {
								cout << "���ķ�����LR(1)�ķ������ڶ��ض�����ڣ�" << endl;
								return 0;
							}
							else table[m] = to_string(c[i].go[next]);
						}
					}
					break;
				}
			}
		}
	}
	return 1;
}




int check(int time, int start_idx, int end_idx) { 

	string sentence;
	for (int i = start_idx; i <= end_idx; i++)
	{
		if (tokens[i].type == TK_KEY) sentence = sentence + (keyword_type_str[atoi(tokens[i].value)]);
		else if (tokens[i].type >= 1 && tokens[i].type <= 4) sentence = sentence + to_string(tokens[i].type);
		else sentence = sentence + tokens[i].value;
	}
	sentence += '#';
	transform(sentence.begin(), sentence.end(), sentence.begin(), ::tolower);

	fstream f("./sentence/sentence_" + to_string(time) + ".txt", ios::out); 
	
	if (!f) {
		cout << "�޷����ļ�sentence.txt" << endl;
	}
	f << "����\t\t״̬ջ\t\t����ջ\t\t���봮\t\t����˵��" << endl;
	
	vector<string> status; 
	vector<char> symbol; 
	
	int step = 1; 
	
	symbol.push_back('#');
	
	
	status.push_back("0");
	
	while (1) {
		f << step++ << "\t\t"; 
		for (unsigned int i = 0; i < status.size(); i++) 
			if (i == 0) f << status[i];
			else f << " " << status[i];
		f << "\t\t";
		for (unsigned int i = 0; i < symbol.size(); i++) 
			if (i == 0) f << symbol[i];
			else f << " " << symbol[i];
		f << "\t\t";
		for (unsigned int i = 0; i < sentence.length(); i++) 
			if (i == 0) f << sentence[i];
			else f << " " << sentence[i];
		f << "\t\t";
		string cur_status = status[status.size() - 1]; 
		char cur_symbol = sentence[0]; 
		string new_status; 
		map<string, char> m;
		m[cur_status] = cur_symbol;
		new_status = table[m];
		if (new_status == "acc") {
			cout << "�����ɹ��������Ϸ���(�������������鿴�ļ�sentence_" << time << ".txt��" << endl << endl;
			f << "acc�������ɹ�" << endl;
			return 1;
		}
		else if (new_status[0] == 's') { //���� table[i,b]=sj��״̬sj��ջ��������һ���ַ�
			status.push_back(new_status.substr(1)); //��ջ��״̬Ҫȥ����һ���ַ���s��
			symbol.push_back(cur_symbol); //����һ���ַ�
			sentence = sentence.substr(1);
			f << "ACTION[" << cur_status << "," << cur_symbol << "]=" << new_status << "����״̬" << new_status << "��ջ" << endl;
		}
		else if (new_status[0] == 'r') { //���� table[i,b]=rj���ò���ʽG(j)��Լ����table[x,y]��ջ
			new_status = new_status.substr(1); //ȥ����r��
			f << "r" << new_status << "����";
			int gid = atoi(new_status.c_str()); //�������ʽ���
			int len = G[gid].size() - 1; //����Լ���ַ�������
			char reduced_symbol = G[gid][0]; //��Լ�õ��ķ��ս��
			for (int i = 0; i < len; i++) {
				status.pop_back(); //��Լ����ȥ��ջ����len��״̬��
				symbol.pop_back();
			}
			map<string, char> m;
			m[status[status.size() - 1]] = reduced_symbol;
			new_status = table[m];
			status.push_back(new_status);
			symbol.push_back(reduced_symbol);
			for (unsigned int i = 0; i < G[gid].size(); i++) {
				if (i == 1) f << "->";
				f << G[gid][i];
			}
			f << "��Լ����GOTO[" << status[status.size() - 1] << "," << reduced_symbol << "]=" << new_status << "��ջ" << endl;
		}
		else
		{
			cur_symbol = '$'; 
			m[cur_status] = cur_symbol;
			new_status = table[m];
			if (new_status == "")
			{
				cout << "��������﷨���󣡣�������鿴���������ļ�sentence_" << time << ".txt��" << endl << endl;
				return 0;
			}
			else
			{
				status.push_back(new_status);
				symbol.push_back('$');
				f << "ACTION[" << cur_status << "," << cur_symbol << "]=" << new_status << "����״̬" << new_status << "��ջ," <<  "��$�����ջ" <<endl;
			}
		}
	}
	f.close();
	cout << "��������﷨���󣡣�������鿴���������ļ�sentence_" << time << ".txt��" << endl << endl;
	return 0;
}



int main(int argc, char** argv) {

	char* str;
	char* input_file;

	if (argc == 1) {
		str = (char*)malloc(strlen(DEFAULT_INPUT) + 1);
		strcpy(str, DEFAULT_INPUT);
	}
	else {
		input_file = argv[1];
		FILE* fp = fopen(input_file, "r");
		if (fp == NULL) {
			fprintf(stderr, "Error: Cannot open file %s\n", input_file);
			return 1;
		}

		fseek(fp, 0, SEEK_END);
		long fsize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		str = (char*)malloc(fsize + 1);
		fread(str, 1, fsize, fp);
		fclose(fp);
		str[fsize] = '\0';
	}

	char* p = str;
	while (*p) {
		p = lex_step(p, &tokens[nr_tokens]);
		if (p == NULL)
			break;
		else if (tokens[nr_tokens].type != TK_SPACE)
			nr_tokens++;
	}

	print_tokens(tokens, nr_tokens);

	free(str);

	read_G();

	cout << endl;

	show_G();

	get_First();

	get_Closure();

	show_Closure();

	if (get_Table()) {

		show_Table();

		cout << endl;


		int time = 0;

		filesystem::remove_all("./sentence");
		filesystem::create_directory("./sentence");

		int start_idx = 0, end_idx = 0;
		while (1) {
			while (end_idx < nr_tokens && tokens[end_idx].type != TK_SEM) 
				end_idx++;
			check(++time, start_idx, end_idx - 1);
			start_idx = ++end_idx;
			if (start_idx >= nr_tokens) break;
		}
	}
	system("pause");
}