#ifndef FUNCTION_H
#define FUNCTION_H

#include <iostream>
#include <stdio.h>
#include <stack>
#include <string>
#include<algorithm>

using namespace std;

#define MAX 100

/********** NFA 结构定义 **********/

// 节点，定义结构体，便于以后拓展
struct state {
	int StateName;
};

// 边，空集以“E”表示
struct edge {
	state StartState;
	state EndState;
	char Symbol;
};

// NFA 单元
struct cell_nfa {
	edge EdgeSet[MAX];
	int EdgeNum;
	int StateNum;		// 现存状态数
	int StateCount;		// 累计出现的状态数，决定状态的编号
	state StartState;
	state EndState;
};

/********** DFA 结构定义 **********/

// 节点，定义结构体，便于以后拓展
struct state_dfa {
	int StateName;
	bool isbegin;
	bool isend;
};

// DFA的边，空集以“E”表示
struct edge_dfa {
	state_dfa StartState;
	state_dfa EndState;
	char Symbol;
};

struct nodeSet
{
	int StateNum;
	state_dfa StateSet[MAX];
};

// DFA 单元
struct cell_dfa
{
	edge_dfa EdgeSet[MAX];
	int EdgeNum;
	int StateNum;
	nodeSet node;
};


/*********** 函数声明 *************/

string pretreatment(string before_string);

string add_join_symbol(string add_string);

string middle_transform_back(string str);

cell_nfa back_transform_NFA(string str);

void Display_nfa(cell_nfa Cell);

cell_dfa NFA_transform_DFA(cell_nfa cell1);

void compare_DFA(cell_dfa cell1, cell_dfa cell2);

#endif // !FUNCTION_H