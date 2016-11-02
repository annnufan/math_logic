#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

ofstream fout("out.out");

string to_string(string s) {
	string ans;
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
		//cout << endl << x % 10 << ' ' <<c << endl; 
		s = c + s;
		x /= 10;	
	}
	if (s == "")
		s = "0";
	return s;
}



struct expression {
	expression(string s) {
		s = to_string(s);
		expr = s;
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
		
		i = 0;
		balance = 0;
		while (i < s.length() && (s[i] != '|' || balance != 0)) {
			if (s[i] == '(')
				balance++;
			if (s[i] == ')')
				balance--;
			i++;
		}
		if (i != s.length()) {
			type = 1;
			first = new expression(s.substr(0, i));
			second = new expression(s.substr(i + 1));
			return;					
		}
		
		i = 0;
		balance = 0;
		while (i < s.length() && (s[i] != '&' || balance != 0)) {
			if (s[i] == '(')
				balance++;
			if (s[i] == ')')
				balance--;
			i++;
		}
		if (i != s.length()) {
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
			*this = expression(s.substr(1, s.length() - 1));
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
	string val, expr;	
};

vector<expression> axioms, evidence, scheme_axiom;

bool operator==(expression& a, expression& b) {
	return to_string(a.expr) == to_string(b.expr); 		
}



void axiom_parse(string s){
	int i = 0;
	//fout << s;
	while (i < s.length() && (s[i] != '|' || s[i + 1] != '-')) {
		string add_s = "";
		while (i < s.length() && s[i] != ',' && (s[i] != '|' || s[i + 1] != '-')) {
			add_s += s[i++];	
		}
		axioms.push_back(expression(add_s));
		//fout << add_s << ' ' << i << ' ' << s[i] << endl;
		if (s[i] == ',') {
			i++;	
		}
	}	
}


bool equal_tree(expression a, expression b, map<string, string> m) {
	if (a.type != b.type && a.type != 4)
		return false;
	if (a.type != 4) {
		bool v = equal_tree(*(a.first), *(b.first), m);
		if (a.type != 3)
			v = v && equal_tree(*(a.second), *(b.second), m);
		return v;
	}
	if (m[a.val] == "") {
		m[a.val] = b.expr;
		return true;		
	}
	return m[a.val] == b.expr;
}



string annotation(string s) {
	expression x(s);
	//fout << x.expr << endl;
	evidence.push_back(x);
		
	for (int i = 0; i < (int)axioms.size(); i++) {
		//fout << axioms[i - 1].expr << ' ' << x.expr << endl;
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
		map<string, string> m;
		if (equal_tree(scheme_axiom[i], x, m)) {
			string a = to_string_int(i + 1);
			return "Сх. акс. " + a;	
		}	
	}
	
	return "Не доказано";
}

int main() {
	string head, s;
	cin >> head;
	expression a(head);
	axiom_parse(head);
	string arr[10] = {"a->b->a", "(a->b)->(a->b->c)->(a->c)", "a->b->a&b", "a&b->a", "a&b->b", "a->a|b", "b->a|b", "(a->c)->(b->c)->(a|b->c)", "(a->b)->(a->!b)->!a", "!!a->a"}; 
	
	for (int i = 0; i < 10; i++) {
		scheme_axiom.push_back(expression(arr[i]));
	}
	
		

	int i = 1;
	fout << to_string(head) << endl;
	while (cin >> s) {
		fout << "(" << i++ << ") " << to_string(s) << " (" << annotation(s) << ")" << endl;	
	}                                                     
	return 0;	
}
