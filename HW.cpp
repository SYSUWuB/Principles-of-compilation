// HW.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "function.h"

int main()
{
	string str1, str2;
	int times;

	cin >> times;
	for (int i = 0; i < times; i++) {
		cin >> str1 >> str2;

		str1 = pretreatment(str1);
		str1 = add_join_symbol(str1);
		str1 = middle_transform_back(str1);
		cell_nfa nfa1 = back_transform_NFA(str1);
		//Display_nfa(nfa1);
		cell_dfa dfa1 = NFA_transform_DFA(nfa1);

		str2 = pretreatment(str2);
		str2 = add_join_symbol(str2);
		str2 = middle_transform_back(str2);
		cell_nfa nfa2 = back_transform_NFA(str2);
		//Display_nfa(nfa2);
		cell_dfa dfa2 = NFA_transform_DFA(nfa2);

		compare_DFA(dfa1, dfa2);
	}

	return 0;
}

