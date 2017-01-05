#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

ifstream fin("in.in");
ofstream fout("out.out");

string s_annotation = "";

string to_string(string s) {
	string ans = "";
	for (int i = 0; i < (int)s.length(); i++) {
		if (s[i] != ' ') 
			ans += s[i];
	}
	return ans;
}

string to_string_int(int x) {
	string s = "";
	while (x > 0) {
		char c = (x % 10) + '0';
		s = c + s;
		x /= 10;	
	}
	if (s == "")
		s = "0";
	return s;
}

struct expression {
	expression() {
		type = -1;	
	}
	
	expression(string s) {
		// fout << s << "\n";
		s = to_string(s);
		val = s;
		int i = 0, balance = 0;
		while (i < (int)s.length() && (s[i] != '-' || balance != 0)) {
			if (s[i] == '(')
				balance++;
			if (s[i] == ')')
				balance--;
			i++;
		}
		if (i != (int)s.length()) {
			type = 0;
			first = new expression(s.substr(0, i));
			second = new expression(s.substr(i + 2));
			return;	
		}
		
		i = (int)s.length() - 1;
		balance = 0;
		while (i >= 0 && (s[i] != '|' || balance != 0)) {
			if (s[i] == '(')
				balance--;
			if (s[i] == ')')
				balance++;
			i--;
		}
		if (i != -1) {
			type = 1;
			first = new expression(s.substr(0, i));
			second = new expression(s.substr(i + 1));
			return;					
		}
		
		i = (int)s.length() - 1;
		balance = 0;
		while (i >= 0 && (s[i] != '&' || balance != 0)) {
			if (s[i] == '(')
				balance--;
			if (s[i] == ')')
				balance++;
			i--;
		}
		
		if (i != -1) {
			type = 2;
			first = new expression(s.substr(0, i));
			second = new expression(s.substr(i + 1));
			return;					
		}
		
		if (s[0] == '!') {
			type = 3;
			first = new expression(s.substr(1));
			second = 0;
			return;	
		}
		if (s[0] == '@') {
			type = 5;
			i = 1;
			while (i < (int)s.length() && ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= '0' && s[i] <= '9'))) 
				i++;
			first = new expression(s.substr(1, i - 1));
			second = new expression(s.substr(i));
			return;
		}
		if (s[0] == '?') {
			type = 6;
			i = 1;
			while (i < (int)s.length() && ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= '0' && s[i] <= '9'))) 
				i++;
			first = new expression(s.substr(1, i - 1));
			second = new expression(s.substr(i));
			return;
		}
		if (s[0] >= 'A' && s[0] <= 'Z') {
			type = 7;
			i = 1;
			while (i < (int)s.length() && ((s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= '0' && s[i] <= '9'))) 
				i++;
			name = s.substr(0, i);
			i++;
			while (i < (int)s.length() - 1) {
				int j = i;
				balance = 0;
				while (j < (int)s.length() - 1 && (s[j] != ',' || balance != 0)) {
					if (s[j] == '(')
						balance++;
					if (s[j] == ')')
						balance--;
					j++;
				}
				arg.push_back(new expression(s.substr(i, j - i)));
				i = ++j;
			}
			return;
		}

		i = 0;
		balance = 0;
		while (i < (int)s.length() && (s[i] != '=' || balance != 0)) {
			if (s[i] == '(')
				balance++;
			if (s[i] == ')')
				balance--;
			i++;
		}
		if (i != (int)s.length()) {
			type = 8;
			first = new expression(s.substr(0, i));
			second = new expression(s.substr(i + 1));
			return;	
		}
		i = 0, balance = 0;
		while (i < (int)s.length() && (s[i] != '+' || balance != 0)) {
			if (s[i] == '(')
				balance++;
			if (s[i] == ')')
				balance--;
			i++;
		}
		if (i != (int)s.length()) {
			type = 9;
			first = new expression(s.substr(0, i));
			second = new expression(s.substr(i + 1));
			return;	
		}
		i = 0, balance = 0;
		while (i < (int)s.length() && (s[i] != '*' || balance != 0)) {
			if (s[i] == '(')
				balance++;
			if (s[i] == ')')
				balance--;
			i++;
		}
		if (i != (int)s.length()) {
			type = 10;
			first = new expression(s.substr(0, i));
			second = new expression(s.substr(i + 1));
			return;	
		}

		if (s[(int)s.length() - 1] == '\'') {
			type = 11;
			first = new expression(s.substr(0, (int)s.length() - 1));
			return;
		}
		if (s[0] == '0') {
			type = 12;
			val = "0";
			first = 0;
			second = 0;
			return;
		}
		if (s[0] >= 'a' && s[0] <= 'z' && s.find("(") < s.length()) {
			type = 13;
			i = 1;
			while (i < (int)s.length() && ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= '0' && s[i] <= '9'))) 
				i++;
			if (i != (int)s.length()) {
				name = s.substr(0, i++);
				while (i < (int)s.length() - 1) {
					int j = i;
					balance = 0;
					while ((j < (int)s.length() - 1) && (s[j] != ',' || balance != 0)) {
						if (s[j] == '(')
							balance++;
						if (s[j] == ')')
							balance--;
						j++;
					}
					arg.push_back(new expression(s.substr(i, j - i)));
					i = ++j;
				}
				return;
			}
		}
		if(s[0] == '(' && s[(int)s.length() - 1] == ')') {
			*this = expression(s.substr(1, (int)s.length() - 2));
			return;		
		}
		
		type = 4;
		val = s;
		first = 0;
		second = 0;			
	}
	
	friend bool operator==(expression a, expression b);


	int type;
	// type meaning:
	// 	** 0 ->
	// 	** 1 |
	// 	** 2 &
	// 	** 3 !
	// 	** 4 var
	// 	** 5 @
	// 	** 6 ?
	// 	** 7 pred with name
	// 	** 8 pred for equal
	// 	** 9 add
	// 	** 10 mul
	// 	** 11 '
	// 	** 12 0
	// 	** 13 func
	int type_in_proof;
	// type_in_proof meaning
	// 	** 0 axiom
	// 	** 1 scheme_axiom
	// 	** 2 MP
	// 	** 3 @
	// 	** 4 ?
	expression *first, *second, *mppart;
	string val, name;
	vector<expression*> arg;	
} *alpha;

bool exact_equal_tree(expression *a, expression *b) {
	// fout << a->val << ' ' << b->val << endl;
	if (a->type != b->type) {
		return false;
	}
	if (a->type == 12) {
		return true;
	}
	if (a->type == 7 || a->type == 13) {
		if ((a->name == b->name) && a->arg.size() == b->arg.size()) {
			bool v = true;
			for (int i = 0; i < (int)a->arg.size(); ++i) 
				v &= exact_equal_tree(a->arg[i], b->arg[i]);
			return v;
		}
		return false;
	}
	if (a->type != 4) {
		bool v = exact_equal_tree(a->first, b->first);
		if (a->type != 3 && a->type != 11)
			v = v && exact_equal_tree(a->second, b->second);
		return v;
	}
	
	return a->val == b->val;
}

vector<expression*> evidence;

bool operator==(expression a, expression b) {
	return exact_equal_tree(&a, &b); 		
}

void annotation(string s) {
	// fout << "START\n";
	expression *x = new expression(s);
	int flag = -1;
	for (int i = 0; i < (int)evidence.size(); i++) {
		expression z = *(evidence[i]);
		if (z == *(x)) {
			flag = i;
			break;
		}
	}
	if (flag == -1)
		fout << s << endl;
	evidence.push_back(x);
}

int main() {
	string head, s;
	while (getline(fin, s)) {
		annotation(s);
	}                                                     
	return 0;	
}