#ifndef FUNCTION_H
#define FUNCTION_H

#include <iostream>
#include <stdio.h>
#include <stack>
#include <string>
#include<algorithm>

using namespace std;

#define MAX 100

/********** NFA �ṹ���� **********/

// �ڵ㣬����ṹ�壬�����Ժ���չ
struct state {
	int StateName;
};

// �ߣ��ռ��ԡ�E����ʾ
struct edge {
	state StartState;
	state EndState;
	char Symbol;
};

// NFA ��Ԫ
struct cell_nfa {
	edge EdgeSet[MAX];
	int EdgeNum;
	int StateNum;		// �ִ�״̬��
	int StateCount;		// �ۼƳ��ֵ�״̬��������״̬�ı��
	state StartState;
	state EndState;
};

/********** DFA �ṹ���� **********/

// �ڵ㣬����ṹ�壬�����Ժ���չ
struct state_dfa {
	int StateName;
	bool isbegin;
	bool isend;
};

// DFA�ıߣ��ռ��ԡ�E����ʾ
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

// DFA ��Ԫ
struct cell_dfa
{
	edge_dfa EdgeSet[MAX];
	int EdgeNum;
	int StateNum;
	nodeSet node;
};


/*********** �������� *************/

string pretreatment(string before_string);

string add_join_symbol(string add_string);

string middle_transform_back(string str);

cell_nfa back_transform_NFA(string str);

void Display_nfa(cell_nfa Cell);

cell_dfa NFA_transform_DFA(cell_nfa cell1);

void compare_DFA(cell_dfa cell1, cell_dfa cell2);

#endif // !FUNCTION_H