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


struct C { //闭包CLOSURE
	
	vector< vector<char> > project; //项目集
	
	vector< set<char> > outlook; //展望串
	
	unordered_map<char, int> go; //GO函数
};
vector<C> c;

//输出拓广文法

void show_G() {
	
	cout << "原文法拓广为文法G[M]：" << endl;
	
	for (unsigned int i = 0; i < G.size(); i++) { 
		
		cout << i << "）";
		
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


//输出非终结符和终结符

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


//输出First集

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



//输出闭包

void show_Closure() {  //项目集和GO函数输出到Closure.txt文件
	fstream f("Closure.txt", ios::out);
	if (!f) {
		cout << "Closure.txt文件打开出错！" << endl;
		return;
	}
	f << "该文法的项目集和GO函数：" << endl;
	for (unsigned int i = 0; i < c.size(); i++) {
		f << "I" << i << ":" << endl;
		for (unsigned int j = 0; j < c[i].project.size(); j++) {
			for (unsigned int k = 0; k < c[i].project[j].size(); k++) {
				if (k == 1) f << "->";
				if (c[i].project[j][k] == ' ') f << "·";
				else f << c[i].project[j][k];
			}
			f << "，";
			for (auto it : c[i].outlook[j]) {
				if (it == *(c[i].outlook[j].begin())) f << it;
				else f << "/" << it;
			}
			f << endl;
		}
		for (auto it : c[i].go) {
			f << "GO(I" << i << "，" << it.first << ") = I" << it.second << endl;
		}
		f << endl;
	}
	cout << "已将项目集和GO函数生成到Closure.txt文件中。" << endl << endl;
}



//输出LR分析表

void show_Table() {  //LR分析表输出到LR_Table.txt文件
	
	fstream f("LR_Table.txt", ios::out);
	
	if (!f) {
		cout << "LR_Table.txt文件打开出错！" << endl;
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
	cout << "已将LR分析表生成到LR_Table.txt文件中。" << endl << endl;
	/*for (auto it1 : table) {
		for (auto it2 : it1.first) {
			cout << it2.first << it2.second << ":"<<it1.second<<endl;
		}
	}*/
}



//读取文法G[S]->G'[M]，并区分终结符和非终结符 

void read_G() { 
	char ch; 
	int i = 0; 
	vector<char> v; 
	char X; 
	set<char> m;
	nts['M'] = m;
	FILE* fp = fopen("production.txt", "r");
	if (fp == NULL) {
		cout << "无法打开文件production.txt" << endl;
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



//计算First集合

void get_First() { 
	
	for (auto& it : ts) it.second.insert(it.first);

	
	
	int r = 0;
	
	int change = 1;
	
	while (change) {
		
		if (r == 20)break;
		
		r++;
		
		change = 0;
		
		for (auto& it : nts) { //对每个非终结符
			for (unsigned int i = 0; i < G.size(); i++) { //遍历产生式
				if (G[i][0] == it.first) {
					unsigned int size = it.second.size(); //操作前First(X)的大小
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



//计算CLOSURE，包括GO

void get_Closure() {  
	
	int i = 0; //闭包编号
	
	C clo; //生成第一个闭包（I0）
	
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
								if (k + t + 1 == c[i].project[j].size() - 1) {  //情况一
									for (auto it : c[i].outlook[j]) m.insert(it);
								}
								else if (ts.find(c[i].project[j][k + t + 2]) != ts.end()) {  //情况二
									m.insert(c[i].project[j][k + 2 + t]);
								}
								else {  //情况三
									set<char> m1((nts.find(c[i].project[j][k + 2 + t]))->second);
									for (auto it : m1) {
										if (it == '$') {  //含空字
											kong = true;
											t++;
										}
										else { //不含空字
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
		i++; //下一闭包
	}
}



//由CLOSURE计算LR(1)分析表table

int get_Table() { 
	
	for (unsigned int i = 0; i < c.size(); i++) { //遍历所有闭包
		
		for (unsigned int j = 0; j < c[i].project.size(); j++) { //遍历每个闭包中的所有项目
			
			for (unsigned int k = 0; k < c[i].project[j].size(); k++) { //扫描该项目,找到当前位置
				
				if (c[i].project[j][k] == ' ') {
					//cout << 1 << endl;
					/*
					if (k == 0 && c[i].project[j][k + 1] == '$') { //形如 【X->·，β】，规约空串
						int id;
						for (unsigned int x = 0; x < G.size(); x++) { //扫描G'[M]找到对应的产生式的编号
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
								cout << "该文法不是LR(1)文法，存在多重定义入口！" << endl;
								return 0;
							}
							else table[m] = (string)"r" + to_string(id);
						}
					}
					else if (k == c[i].project[j].size() - 1 && c[i].project[j][k - 1] == '$') { //形如 【X->$·，β】，规约空串
                        int id;
						for (unsigned int x = 0; x < G.size(); x++) { //扫描G'[M]找到对应的产生式的编号
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
								cout << "该文法不是LR(1)文法，存在多重定义入口！" << endl;
								return 0;
							}
							else table[m] = (string)"r" + to_string(id);
						}
                    }*/
					if (k == c[i].project[j].size() - 1) { //形如 【X->α·，β】，归约/acc
						
						if (c[i].project[j][0] == 'M') {  //形如 【M->X·，#】，令table[i,#]=acc
							map<string, char> m;
							m[to_string(i)] = '#';
							
							if (table.find(m) != table.end() && table[m] != "acc") {
								cout << "该文法不是LR(1)文法，存在多重定义入口！" << endl;
								return 0;
							}
							else table[m] = "acc";
						}
						else { //形如 【X->α·，a】，归约，令table[i,a]=rj
							int id;
							for (unsigned int x = 0; x < G.size(); x++) { //扫描G'[M]找到对应的产生式的编号
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
									cout << "该文法不是LR(1)文法，存在多重定义入口！" << endl;
									return 0;
								}
								else table[m] = (string)"r" + to_string(id);
							}
						}
					}
					else { //形如 【X->α·β，γ】
						char next = c[i].project[j][k + 1];
						if (ts.find(next) != ts.end()) {  //形如 【X->α·aβ，γ】,令table[i,a]=sj
							map<string, char> m;
							m[to_string(i)] = next;
							if (table.find(m) != table.end() && table[m] != (string)"s" + to_string(c[i].go[next])) {
								cout << "该文法不是LR(1)文法，存在多重定义入口！" << endl;
								return 0;
							}
							else table[m] = (string)"s" + to_string(c[i].go[next]);
						}
						else { //形如 【X->α·Yβ，γ】,令table[i,Y]=j
							map<string, char> m;
							m[to_string(i)] = next;
							if (table.find(m) != table.end() && table[m] != to_string(c[i].go[next])) {
								cout << "该文法不是LR(1)文法，存在多重定义入口！" << endl;
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
		cout << "无法打开文件sentence.txt" << endl;
	}
	f << "步骤\t\t状态栈\t\t符号栈\t\t输入串\t\t动作说明" << endl;
	
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
			cout << "分析成功，该语句合法！(具体分析过程请查看文件sentence_" << time << ".txt）" << endl << endl;
			f << "acc：分析成功" << endl;
			return 1;
		}
		else if (new_status[0] == 's') { //形如 table[i,b]=sj，状态sj入栈，并读入一个字符
			status.push_back(new_status.substr(1)); //入栈的状态要去掉第一个字符‘s’
			symbol.push_back(cur_symbol); //读入一个字符
			sentence = sentence.substr(1);
			f << "ACTION[" << cur_status << "," << cur_symbol << "]=" << new_status << "，即状态" << new_status << "入栈" << endl;
		}
		else if (new_status[0] == 'r') { //形如 table[i,b]=rj，用产生式G(j)归约，且table[x,y]入栈
			new_status = new_status.substr(1); //去掉‘r’
			f << "r" << new_status << "：用";
			int gid = atoi(new_status.c_str()); //计算产生式编号
			int len = G[gid].size() - 1; //被归约的字符串长度
			char reduced_symbol = G[gid][0]; //归约得到的非终结符
			for (int i = 0; i < len; i++) {
				status.pop_back(); //归约，即去掉栈顶的len个状态项
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
			f << "归约，且GOTO[" << status[status.size() - 1] << "," << reduced_symbol << "]=" << new_status << "入栈" << endl;
		}
		else
		{
			cur_symbol = '$'; 
			m[cur_status] = cur_symbol;
			new_status = table[m];
			if (new_status == "")
			{
				cout << "该语句有语法错误！（详情请查看分析过程文件sentence_" << time << ".txt）" << endl << endl;
				return 0;
			}
			else
			{
				status.push_back(new_status);
				symbol.push_back('$');
				f << "ACTION[" << cur_status << "," << cur_symbol << "]=" << new_status << "，即状态" << new_status << "入栈," <<  "且$入符号栈" <<endl;
			}
		}
	}
	f.close();
	cout << "该语句有语法错误！（详情请查看分析过程文件sentence_" << time << ".txt）" << endl << endl;
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