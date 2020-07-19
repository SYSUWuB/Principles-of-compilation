#include "function.h"

int STATE_NUM = 0;
int NAME = 0;
int dig_num;
char digital[26];

/*********************************************
*
* 1、“a+”转换为“aa*”，“a?”转换为“(E|a)”
*
**********************************************/
string pretreatment(string before_string) {
	int i = -1;
	int length = before_string.size();
	int return_string_length = 0;
	char* return_string = new char[3 * length];
	char first, second;

	for (i = 0; i < length - 1; i++) {
		first = before_string[i];
		second = before_string[i + 1];

		if (second == '+') {
			return_string[return_string_length++] = first;

			// 转换a+/(...)+
			if (first == ')') {
				int loc = i - 1;
				int num = 1;
				string tem = "";
				while (num != 0) {
					tem += return_string[loc];
					if (return_string[loc] == ')')num++;
					if (return_string[loc] == '(')num--;
					loc--;
				}
				int len = tem.size();
				for (int j = 0; j < len; j++)
					return_string[return_string_length++] = tem[len - 1 - j];
			}

			return_string[return_string_length++] = first;
			return_string[return_string_length++] = '*';
		}
		// 转换a?/(...)?
		else if (second == '?') {
			if (first == ')') {
				int loc = i - 1;
				int num = 1;
				string tem = "";
				while (num != 0) {
					tem += before_string[loc];
					if (before_string[loc] == ')')num++;
					if (before_string[loc] == '(')num--;
					loc--;
				}
				int len = tem.size();

				return_string_length -= len;
				return_string[return_string_length++] = '(';
				return_string[return_string_length++] = 'E';
				return_string[return_string_length++] = '|';
				for (int j = 0; j < len; j++)
					return_string[return_string_length++] = tem[len - 1 - j];
			}
			else {
				return_string[return_string_length++] = '(';
				return_string[return_string_length++] = 'E';
				return_string[return_string_length++] = '|';
			}
			return_string[return_string_length++] = first;
			return_string[return_string_length++] = ')';
		}
		else if (first != '+' && first != '?')
			return_string[return_string_length++] = first;
	}
	if (second != '+' && second != '?')
		return_string[return_string_length++] = second;

	// 检查输入的字母个数
	bool is_have[26] = { false };
	for (i = 0; i < before_string.size(); i++) {
		first = before_string[i];
		if (first >= 'a' && first <= 'z')
			is_have[first - 97] = true;
	}
	dig_num = 0;
	for (i = 0; i < 26; i++) {
		if (is_have[i]) {
			digital[dig_num++] = i + 97;
		}
	}

	string str(return_string);
	return str.substr(0, return_string_length);
}


/*********************************************
*
* 2、正则表达式转换为中缀式，添加“-”以连接
*
**********************************************/
string add_join_symbol(string add_string) {
	int i;
	int length = add_string.size();
	int return_string_length = 0;
	char* return_string = new char[2 * length];
	char first, second;

	for (i = 0; i < length; i++) {
		first = add_string[i];
		second = add_string[i + 1];
		return_string[return_string_length++] = first;

		// 要添加的可能性如ab、*b、a(、)b等情况
		// 若第二个是字母、第一个不是“(”、“|”都要添加
		if (first != '(' && first != '|' && second <= 'z' && second >= 'a') {
			return_string[return_string_length++] = '-';
		}
		// 若第二个是“(”，第一个不“|”、“(”，也要加
		else if (second == '(' && first != '|' && first != '(') {
			return_string[return_string_length++] = '-';
		}

	}
	if (i == length - 1) {
		return_string[return_string_length++] = second;
	}

	string str(return_string);
	return str.substr(0, return_string_length);
}


/****************
*
* 3.1、栈内优先级，栈顶的字符的优先级
*
****************/
int isp(char c) {
	switch (c)
	{
	case '#':
		return 0;
	case '(':
		return 1;
	case '*':
		return 7;
	case '|':
		return 3;
	case '-':
		return 5;
	case ')':
		return 9;
	}
	cerr << "ERROR!" << endl;
	return false;
}


/****************
*
* 3.2、栈外优先级，当前扫描到的字符的优先级
*
****************/
int icp(char c) {
	switch (c)
	{
	case '#':
		return 0;
	case '(':
		return 9;
	case '*':
		return 6;
	case '|':
		return 2;
	case '-':
		return 4;
	case ')':
		return 1;
	}
	cerr << "ERROR!" << endl;
	return false;
}


/*********************************************
*
* 3、中缀表达式转换为后缀表达式
*
**********************************************/
string middle_transform_back(string str) {
	// 添加e的最后一个字符为结束符“#”，将其放到栈s的底部
	str = str + '#';

	stack<char> s;
	s.push('#');

	string out = "";
	int read = 0;
	char ch = str[read++], ch1, op;
	while (!s.empty()) {
		if ((ch <= 'z' && ch >= 'a') || ch == 'E') {
			out = out + ch;
			ch = str[read++];
		}
		else {
			ch1 = s.top();
			if (isp(ch1) < icp(ch)) {
				s.push(ch);
				ch = str[read++];
			}
			else if (isp(ch1) > icp(ch)) {
				op = s.top();
				s.pop();
				out = out + op;
			}
			else {
				op = s.top();
				s.pop();
				if (op == '(')
					ch = str[read++];
			}
		}
	}

	return out;
}


/****************
*
* 4.1、产生一个新的状态节点，便于管理
*      添加到state_set[]数组中
*
****************/
state new_stateNode() {
	state new_state;
	new_state.StateName = NAME;
	STATE_NUM++;
	NAME++;
	return new_state;
}


/****************
*
* 4.2、将一个单元的边的集合复制给另一个单元
*
****************/
void cell_copy(cell_nfa& new_cell, cell_nfa old_cell) {
	int new_num = new_cell.EdgeNum;
	int old_num = old_cell.EdgeNum;
	for (int i = 0; i < old_num; i++) {
		new_cell.EdgeSet[new_num + i] = old_cell.EdgeSet[i];
	}
	new_cell.EdgeNum = new_num + old_num;
}


/****************
*
* 4.3、处理a/E:   -> |start| -ch-> |end|
*      新建一个单元
*
****************/
cell_nfa do_cell(char ch) {
	cell_nfa cell;

	state start = new_stateNode();
	state end = new_stateNode();

	cell.EdgeSet[0].StartState = start;
	cell.EdgeSet[0].EndState = end;
	cell.EdgeSet[0].Symbol = ch;
	cell.EdgeNum = 1;
	cell.StartState = start;
	cell.EndState = end;

	return cell;
}


/****************
*
* 4.4、处理a|b:   -> |start| -E*2-> |N(a)/N(b)| -E*2-> |end|
*      新建一个单元，整合N(a)和N(b)，并且再加4条边
*
****************/
cell_nfa do_or(cell_nfa up, cell_nfa down) {
	cell_nfa cell;
	cell.EdgeNum = 0;

	state start = new_stateNode();
	state end = new_stateNode();

	// 整合两个单元
	cell_copy(cell, up);
	cell_copy(cell, down);

	// 新增4条边
	cell.EdgeSet[cell.EdgeNum].StartState = start;
	cell.EdgeSet[cell.EdgeNum].EndState = up.StartState;
	cell.EdgeSet[cell.EdgeNum++].Symbol = 'E';

	cell.EdgeSet[cell.EdgeNum].StartState = start;
	cell.EdgeSet[cell.EdgeNum].EndState = down.StartState;
	cell.EdgeSet[cell.EdgeNum++].Symbol = 'E';

	cell.EdgeSet[cell.EdgeNum].StartState = up.EndState;
	cell.EdgeSet[cell.EdgeNum].EndState = end;
	cell.EdgeSet[cell.EdgeNum++].Symbol = 'E';

	cell.EdgeSet[cell.EdgeNum].StartState = down.EndState;
	cell.EdgeSet[cell.EdgeNum].EndState = end;
	cell.EdgeSet[cell.EdgeNum++].Symbol = 'E';

	cell.StartState = start;
	cell.EndState = end;

	return cell;
}


/****************
*
* 4.5、处理ab
*      整合N(a)和N(b)，把b的起始状态和a的接受状态进行合并
*
****************/
cell_nfa do_join(cell_nfa left, cell_nfa right) {
	//修改b中的起始状态
	for (int i = 0; i < right.EdgeNum; i++) {
		if (right.EdgeSet[i].StartState.StateName == right.StartState.StateName) {
			right.EdgeSet[i].StartState = left.EndState;
			STATE_NUM--;
		}
		else if (right.EdgeSet[i].EndState.StateName == right.StartState.StateName) {
			right.EdgeSet[i].EndState = left.EndState;
			STATE_NUM--;
		}
	}
	right.StartState = left.EndState;
	left.EndState = right.EndState;

	cell_copy(left, right);
	return left;
}


/****************
*
* 4.6、处理a*:    -> |start| -E1->|i| -a-> |j| -E2-> |end| + -> |start| -E3-> |end| + |j| -E4-> |i|
*      新建单元，整合N(a)，并增加4条边
*
****************/
cell_nfa do_mul(cell_nfa Na) {
	cell_nfa cell;
	cell.EdgeNum = 0;

	state start = new_stateNode();
	state end = new_stateNode();

	// 整合单元
	cell_copy(cell, Na);

	// 新增四条边
	cell.EdgeSet[cell.EdgeNum].StartState = start;
	cell.EdgeSet[cell.EdgeNum].EndState = Na.StartState;
	cell.EdgeSet[cell.EdgeNum++].Symbol = 'E';

	cell.EdgeSet[cell.EdgeNum].StartState = Na.EndState;
	cell.EdgeSet[cell.EdgeNum].EndState = end;
	cell.EdgeSet[cell.EdgeNum++].Symbol = 'E';

	cell.EdgeSet[cell.EdgeNum].StartState = start;
	cell.EdgeSet[cell.EdgeNum].EndState = end;
	cell.EdgeSet[cell.EdgeNum++].Symbol = 'E';

	cell.EdgeSet[cell.EdgeNum].StartState = Na.EndState;
	cell.EdgeSet[cell.EdgeNum].EndState = Na.StartState;
	cell.EdgeSet[cell.EdgeNum++].Symbol = 'E';

	cell.StartState = start;
	cell.EndState = end;

	return cell;
}


/*********************************************
*
* 4、后缀表达式转换为NFA
*
**********************************************/
cell_nfa back_transform_NFA(string str) {
	int length = str.size();
	char ch;
	cell_nfa cell, left, right;
	stack<cell_nfa> Stack;

	for (int i = 0; i < length; i++) {
		ch = str[i];

		switch (ch) {
		case '|':
			right = Stack.top();
			Stack.pop();
			left = Stack.top();
			Stack.pop();
			cell = do_or(left, right);
			Stack.push(cell);
			break;
		case '*':
			left = Stack.top();
			Stack.pop();
			cell = do_mul(left);
			Stack.push(cell);
			break;
		case '-':
			right = Stack.top();
			Stack.pop();
			left = Stack.top();
			Stack.pop();
			cell = do_join(left, right);
			Stack.push(cell);
			break;
		default:
			cell = do_cell(ch);
			Stack.push(cell);
			break;
		}
	}

	cell = Stack.top();
	Stack.pop();

	cell.StateCount = NAME;
	cell.StateNum = STATE_NUM;
	STATE_NUM = 0;
	NAME = 0;

	return cell;
}

void Display_nfa(cell_nfa Cell) {
	cout << "NFA的边数：" << Cell.EdgeNum << endl;
	cout << "NFA的现存状态数：" << Cell.StateNum << endl;
	cout << "NFA的累计状态数：" << Cell.StateCount << endl;
	cout << "NFA的起始状态：" << Cell.StartState.StateName << endl;
	cout << "NFA的结束状态：" << Cell.EndState.StateName << endl;

	for (int i = 0; i < Cell.EdgeNum; i++) {
		cout << "第" << i + 1 << "条边的起始状态：" << Cell.EdgeSet[i].StartState.StateName << " 结束状态：" <<
			Cell.EdgeSet[i].EndState.StateName << " 转换符：" << Cell.EdgeSet[i].Symbol << endl;
	}
}

/****************
*
* 5.1、比较两个状态的NFA状态是否相等
*
****************/
bool compare_state(int* s1, int* s2, int len) {
	for (int i = 0; i < len; i++)
		if (s1[i] != s2[i])
			return false;
	return true;
}


/****************
*
* 5.2、复制节点
*
****************/
void copy_state(state_dfa& d, state_dfa& s) {
	d.isbegin = s.isbegin;
	d.isend = s.isend;
	d.StateName = s.StateName;
}

void Display_dfa(cell_dfa Cell, nodeSet node) {
	cout << "DFA的边数：" << Cell.EdgeNum << endl;
	for (int i = 0; i < node.StateNum; i++) {
		if (node.StateSet[i].isbegin)
			cout << "开始状态为：" << node.StateSet[i].StateName << endl;
	}
	for (int i = 0; i < node.StateNum; i++) {
		if (node.StateSet[i].isend)
			cout << "结束状态为：" << node.StateSet[i].StateName << endl;
	}

	for (int i = 0; i < Cell.EdgeNum; i++) {
		cout << "第" << i + 1 << "条边的起始状态：" << Cell.EdgeSet[i].StartState.StateName << " 结束状态：" <<
			Cell.EdgeSet[i].EndState.StateName << " 转换符：" << Cell.EdgeSet[i].Symbol << endl;
	}
}

/*********************************************
*
* 5、NFA转换为DFA
*
**********************************************/
bool not_in(int* a, int len, int b) {
	if (len == 0)
		return true;
	for (int i = 0; i < len; i++)
		if (a[i] == b)return false;
	return true;
}


cell_dfa NFA_transform_DFA(cell_nfa cell1) {
	cell_dfa cell2;
	cell2.EdgeNum = 0;
	nodeSet node;

	int i, j, k, h;
	int current = 0, count, current_state;
	int* initial = new int[100];
	int* num_nfa = new int[100];
	int** NfaState = new int* [100];
	for (i = 0; i < 100; i++)
		NfaState[i] = new int[100];

	// 获得初始状态及其对应的nfa状态
	initial[0] = cell1.StartState.StateName;
	count = 1;
	for (i = 0; i < count; i++) {
		for (k = 0; k < cell1.EdgeNum; k++) {
			if (cell1.EdgeSet[k].StartState.StateName == initial[i] && cell1.EdgeSet[k].Symbol == 'E' && not_in(initial, count, cell1.EdgeSet[k].EndState.StateName))
				initial[count++] = cell1.EdgeSet[k].EndState.StateName;
		}
	}
	sort(initial, initial + count);

	node.StateNum = 1;
	node.StateSet[0].StateName = 0;
	node.StateSet[0].isbegin = true;
	node.StateSet[0].isend = false;
	for (i = 0; i < count; i++)
		if (initial[i] == cell1.EndState.StateName)
			node.StateSet[0].isend = true;

	num_nfa[0] = count;
	for (i = 0; i < count; i++)
		NfaState[0][i] = initial[i];

	while (current < node.StateNum) {

		// 每个出现的字母
		for (i = 0; i < dig_num; i++) {
			count = 0;

			// 检查直接相连的状态
			for (j = 0; j < num_nfa[current]; j++) {
				current_state = NfaState[current][j];
				for (h = 0; h < cell1.EdgeNum; h++) {
					if (cell1.EdgeSet[h].StartState.StateName == current_state
						&& cell1.EdgeSet[h].Symbol == digital[i]
						&& not_in(initial, count, cell1.EdgeSet[h].EndState.StateName)) {
						initial[count++] = cell1.EdgeSet[h].EndState.StateName;
					}
				}
			}

			// 补齐所有连接的空集转移状态
			for (j = 0; j < count; j++) {
				for (k = 0; k < cell1.EdgeNum; k++) {
					if (cell1.EdgeSet[k].StartState.StateName == initial[j] && cell1.EdgeSet[k].Symbol == 'E' 
						&& not_in(initial, count, cell1.EdgeSet[k].EndState.StateName))
						initial[count++] = cell1.EdgeSet[k].EndState.StateName;
				}
			}
			sort(initial, initial + count);

			if (count != 0) {
				// 检查是否已有该DFA状态
				for (j = 0; j < node.StateNum; j++) {
					if (num_nfa[j] != count)
						continue;
					if (compare_state(NfaState[j], initial, count))
						break;
				}
				// 新状态
				if (j == node.StateNum) {
					k = node.StateNum;
					node.StateNum++;
					node.StateSet[k].StateName = k;
					node.StateSet[k].isbegin = false;
					node.StateSet[k].isend = false;
					num_nfa[k] = count;
					for (h = 0; h < count; h++) {
						NfaState[k][h] = initial[h];
						if (initial[h] == cell1.EndState.StateName)
							node.StateSet[k].isend = true;
					}

				}

				// 已有状态，新建边
				copy_state(cell2.EdgeSet[cell2.EdgeNum].StartState, node.StateSet[current]);
				copy_state(cell2.EdgeSet[cell2.EdgeNum].EndState, node.StateSet[j]);
				cell2.EdgeSet[cell2.EdgeNum++].Symbol = digital[i];
			}
	
		}

		current++;
	}

	//Display_dfa(cell2, node);
	cell2.StateNum = node.StateNum;
	cell2.node = node;

	delete[] initial;
	delete[] num_nfa;
	for (i = 0; i < 100; i++)
		delete[] NfaState[i];
	delete[] NfaState;

	return cell2;
}


/*********************************************
*
* 6、比较DFA的关系
*
**********************************************/

// 求得DFA的补集
nodeSet complement(nodeSet node) {
	nodeSet new_node;
	new_node.StateNum = node.StateNum;

	for (int i = 0; i < node.StateNum; i++) {
		new_node.StateSet[i].StateName = node.StateSet[i].StateName;
		new_node.StateSet[i].isbegin = node.StateSet[i].isbegin;
		new_node.StateSet[i].isend = !node.StateSet[i].isend;
	}

	return new_node;
}

// 判断是否都为接受状态
bool is_end(int x1, int x2, nodeSet node1, nodeSet node2) {
	
	if (node1.StateSet[x1].isend && node2.StateSet[x2].isend)
		return true;

	else return false;
	
}

// 判断两个DFA是否有公共的子串。有则包含，无则不包含
bool if_intersect(int** A, int** B, int x1, int x2, nodeSet node1, nodeSet node2, bool** visit) {

	visit[x1][x2] = true;
	if (is_end(x1, x2, node1, node2)) return true;

	for (int i = 0; i < 26; i++) {
		int next_a = A[x1][i];
		int next_b = B[x2][i];

		if (!visit[next_a][next_b]) {
			if (if_intersect(A, B, next_a, next_b, node1, node2, visit))
				return true;
			visit[next_a][next_b] = false;
		}
	}

	return false;
}

void compare_DFA(cell_dfa cell1, cell_dfa cell2) {
	int i, j, h;

	// 添加孤岛状态
	nodeSet node1 = cell1.node;
	h = node1.StateNum;
	node1.StateNum++;
	node1.StateSet[h].StateName = h;
	node1.StateSet[h].isbegin = false;
	node1.StateSet[h].isend = false;

	int** matrix1 = new int* [node1.StateNum];
	for (i = 0; i < node1.StateNum; i++)
		matrix1[i] = new int[27];
	for (i = 0; i < node1.StateNum; i++) {
		for (j = 0; j < 27; j++) {
			matrix1[i][j] = h;
		}
	}
	for (i = 0; i < cell1.EdgeNum; i++) {
		matrix1[cell1.EdgeSet[i].StartState.StateName][cell1.EdgeSet[i].Symbol - 'a'] = cell1.EdgeSet[i].EndState.StateName;
	}

	nodeSet node2 = cell2.node;
	h = node2.StateNum;
	node2.StateNum++;
	node2.StateSet[h].StateName = h;
	node2.StateSet[h].isbegin = false;
	node2.StateSet[h].isend = false;

	int** matrix2 = new int* [node2.StateNum];
	for (i = 0; i < node2.StateNum; i++)
		matrix2[i] = new int[27];
	for (i = 0; i < node2.StateNum; i++) {
		for (j = 0; j < 27; j++) {
			matrix2[i][j] = h;
		}
	}
	for (i = 0; i < cell2.EdgeNum; i++) {
		matrix2[cell2.EdgeSet[i].StartState.StateName][cell2.EdgeSet[i].Symbol - 'a'] = cell2.EdgeSet[i].EndState.StateName;
	}

	/*for (i = 0; i < node1.StateNum; i++) {
		for (j = 0; j < 27; j++) {
			cout << matrix1[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	for (i = 0; i < node2.StateNum; i++) {
		for (j = 0; j < 27; j++) {
			cout << matrix2[i][j] << " ";
		}
		cout << endl;
	}*/

	bool** visit = new bool* [200];
	for (i = 0; i < 200; i++)
		visit[i] = new bool[200];

	nodeSet node3 = complement(node1);
	nodeSet node4 = complement(node2);

	for (i = 0; i < 200; i++)
		for (j = 0; j < 200; j++)
			visit[i][j] = false;

	int ans1 = if_intersect(matrix1, matrix2, 0, 0, node3, node2, visit);

	for (i = 0; i < 200; i++)
		for (j = 0; j < 200; j++)
			visit[i][j] = false;

	int ans2 = if_intersect(matrix1, matrix2, 0, 0, node1, node4, visit);

	if (ans1 && ans2)cout << "!";
	else if (ans1)cout << "<";
	else if (ans2)cout << ">";
	else cout << "=";
	cout << endl;

	for (i = 0; i < 200; i++)
		delete[] visit[i];
	for (i = 0; i < node1.StateNum; i++)
		delete[] matrix1[i];
	for (i = 0; i < node2.StateNum; i++)
		delete[] matrix2[i];
	delete[] visit;
	delete[] matrix1;
	delete[] matrix2;
}
