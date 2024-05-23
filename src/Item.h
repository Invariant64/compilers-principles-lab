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

//返回s的第一个词
string firstWord(string s)
{
    s += " ";
    string first = s.substr(0, s.find(" "));
    return first;
}

//将字符串划分为一个个词
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
    string item;//项目
    string left;//项目左部
    string right;//项目右部
    string symbol;//向前搜索符号
    static int count;

public:
    int id;

    //参数是产生式

    Item(string i)
    {
        id = count++;
        left = i.substr(0, i.find("->"));
        right = i.substr(i.find("->") + 2);
        item = left + "->" + right;
        symbol = "$";//初始向前搜索符为"$"

        if (right.find(".") == string::npos)
            addDot(0);
    }

    //参数是左部和右部
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

    //参数是左部和右部和向前搜索符号
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
    //找点的位置
    int getDot(string item)
    {
        return item.find(".");
    }
    //设置向前搜索符号
    void setSymbol(string new_symbol)
    {
        symbol = new_symbol;
    }
    //给文法加点
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

    //判断一个项目进度是否到结尾
    int hasNextDot()
    {
        vector<string>buffer = split(right, ".");
        if (buffer.size() > 1)
            return 1;
        else
            return 0;
    }

    //得到"."后面的一个文法符号
    string getPath()
    {
        vector<string>buffer = split(item, ".");
        buffer[1].erase(0, 1);
        string first = firstWord(buffer[1]);
        return first;
    }

    //返回下一个点的串
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
