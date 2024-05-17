#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <cstring>
#include <algorithm>
#include <set>
#include <map>
#include <stack>
#include <utility>
#include <iomanip>
using namespace std;

//����s�ĵ�һ����
string firstWord(string s)
{
    s += " ";
    string first = s.substr(0, s.find(" "));
    return first;
}

//���ַ�������Ϊһ������
vector<string> split(string s, string separator)
{
    vector<string>v;

    string::size_type pos1, pos2;
    pos2 = s.find(separator);
    pos1 = 0;

    while (string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2 - pos1));

        pos1 = pos2 + separator.size();
        pos2 = s.find(separator, pos1);
    }
    if (pos1 != s.length())
        v.push_back(s.substr(pos1));

    return v;
}

class Item
{
private:
    string item;//��Ŀ
    string left;//��Ŀ��
    string right;//��Ŀ�Ҳ�
    string symbol;//��ǰ��������
    static int count;

public:
    int id;

    //�����ǲ���ʽ

    Item(string i)
    {
        id = count++;
        left = i.substr(0, i.find("->"));
        right = i.substr(i.find("->") + 2);
        item = left + "->" + right;
        symbol = "$";//��ʼ��ǰ������Ϊ"$"

        if (right.find(".") == string::npos)
            addDot(0);
    }

    //�������󲿺��Ҳ�
    Item(string l, string r)
    {
        id = count++;
        left = l;
        right = r;
        symbol = "$";
        item = left + "->" + right;

        if (right.find(".") == string::npos)
            addDot(0);
    }

    //�������󲿺��Ҳ�����ǰ��������
    Item(string l, string r, string s)
    {
        id = count++;
        left = l;
        right = r;
        symbol = s;
        item = left + "->" + right;

        if (right.find(".") == string::npos)
            addDot(0);
    }

    string getLeft()
    {
        return left;
    }

    string getRight()
    {
        return right;
    }

    string getItem()
    {
        item = left + "->" + right;
        return item;
    }

    string getSymbol()
    {
        return symbol;
    }
    //�ҵ��λ��
    int getDot(string item)
    {
        return item.find(".");
    }
    //������ǰ��������
    void setSymbol(string new_symbol)
    {
        symbol = new_symbol;
    }
    //���ķ��ӵ�
    void addDot(int pos)
    {
        if (right[pos] == '@')
            right = ".";
        else if (pos == 0)
            right.insert(pos, ". ");
        else if (pos == right.size())
            right.insert(pos, " .");
        else
            right.insert(pos, " . ");
    }

    //�ж�һ����Ŀ�����Ƿ񵽽�β
    int hasNextDot()
    {
        vector<string>buffer = split(right, ".");
        if (buffer.size() > 1)
            return 1;
        else
            return 0;
    }

    //�õ�"."�����һ���ķ�����
    string getPath()
    {
        vector<string>buffer = split(item, ".");
        buffer[1].erase(0, 1);
        string first = firstWord(buffer[1]);
        return first;
    }

    //������һ����Ĵ�
    string nextDot()
    {
        int dotPos = right.find(".");
        vector<string>buffer = split(item, ".");
        buffer[1].erase(0, 1);
        string first = firstWord(buffer[1]);
        int nextPos = dotPos + first.size();
        right.erase(right.find("."), 2);
        right.insert(nextPos, " .");
        return right;
    }

    bool operator ==(Item& x)
    {
        return getItem() == x.getItem();
    }
};
int Item::count = 0;
#pragma once
