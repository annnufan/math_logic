#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

ifstream fin("in.in");
ofstream fout("out.out");

string to_string(string s) {
	string ans = "";
	for (int i = 0; i < s.length(); i++) {
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
		val = "";	
	}
	
	expression(string s) {
		s = to_string(s);
		int i = 0, balance = 0;
		while (i < s.length() && (s[i] != '-' || balance != 0)) {
			if (s[i] == '(')
				balance++;
			if (s[i] == ')')
				balance--;
			i++;
		}
		if (i != s.length()) {
			type = 0;
			first = new expression(s.substr(0, i));
			second = new expression(s.substr(i + 2));
			return;	
		}
		
		i = s.length() - 1;
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
		
		i = s.length() - 1;
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
			second = nullptr;
			return;	
		}
		if(s[0] == '(') {
			*this = expression(s.substr(1, s.length() - 2));
			return;		
		}
		
		type = 4;
		val = s;
		first = nullptr;
		second = nullptr;			
	}
	
	friend bool operator==(expression& a, expression& b);


	int type;
	expression *first, *second;
	string val;	
};

bool exact_equal_tree(expression a, expression b) {
	if (a.type != b.type) {
		return false;
	}
	if (a.type != 4) {
		bool v = exact_equal_tree(*(a.first), *(b.first));
		if (a.type != 3)
			v = v && exact_equal_tree(*(a.second), *(b.second));
		return v;
	}
	
	return a.val == b.val;
}

vector<expression> axioms, evidence, scheme_axiom;

bool operator==(expression& a, expression& b) {
	return exact_equal_tree(a, b); 		
}

void axiom_parse(string s){
	int i = 0;
	while (i < s.length() && (s[i] != '|' || s[i + 1] != '-')) {
		string add_s = "";
		while (i < s.length() && s[i] != ',' && (s[i] != '|' || s[i + 1] != '-')) {
			add_s += s[i++];	
		}
		axioms.push_back(expression(add_s));
		if (s[i] == ',') {
			i++;	
		}
	}	
}


bool equal_tree(expression a, expression b, map<string, expression>& m) {
	if (a.type != b.type && a.type != 4) {
		return false;
	}
	if (a.type != 4) {
		bool v = equal_tree(*(a.first), *(b.first), m);
		if (a.type != 3)
			v = v && equal_tree(*(a.second), *(b.second), m);
		return v;
	}
	
	if (m[a.val].val == "") {
		m[a.val] = b;
		return true;		
	}
	return m[a.val] == b;
}



string annotation(string s) {
	expression x(s);
	evidence.push_back(x);

	for (int i = 0; i < (int)axioms.size(); i++) {
		if (axioms[i] == x) {
			string a = to_string_int(i + 1);
			return "Предп. " + a;
		}
	}
	
	for (int i = 0; i < evidence.size() - 1; i++) {
		auto v = evidence[i];
		if (v.type == 0 && *(v.second) == x) {
			for (int j = 0; j < evidence.size()  -1; j++) {
				if (evidence[j] == *(v.first))  {
					string a = to_string_int(j + 1);
					string b = to_string_int(i + 1);
					return "M.P. " + a + ", " + b;
					
				}
			}
		}
	}
	
	
	for (int i = 0; i < 10; i++) {
		map<string, expression> m;
		if (equal_tree(scheme_axiom[i], x, m)) {
			string a = to_string_int(i + 1);
			return "Сх. акс. " + a;	
		}	
	}      
	
	return "Не доказано";
}

int main() {
	string head, s;
	getline(fin, head);
	expression a(head);
	axiom_parse(head);
	string arr[10] = {"a->b->a", "(a->b)->(a->b->c)->(a->c)", "a->b->a&b", "a&b->a", "a&b->b", "a->a|b", "b->a|b", "(a->c)->(b->c)->(a|b->c)", "(a->b)->(a->!b)->!a", "!!a->a"}; 
	
	for (int i = 0; i < 10; i++) {
		scheme_axiom.push_back(expression(arr[i]));
	}     
	
	int i = 1;
	fout << to_string(head) << endl;
	while (getline(fin, s)) {
		fout << "(" << i++ << ") " << to_string(s) << " (" << annotation(s) << ")" << endl;	
	}                                                     
	return 0;	
}