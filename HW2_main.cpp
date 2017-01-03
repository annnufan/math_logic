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

vector<expression*> axioms, evidence, scheme_axiom;

bool operator==(expression a, expression b) {
	return exact_equal_tree(&a, &b); 		
}

void axiom_parse(string s) {
	int i = 0;
	int balance = 0;
	while (i < (int)s.length() && (s[i] != '|' || s[i + 1] != '-')) {
		// fout << s << ' ' << i << endl;
		string add_s = "";
		while (i < (int)s.length() && (s[i] != ',' || balance != 0) && (s[i] != '|' || s[i + 1] != '-')) {
			if (s[i] == '(')
				balance--;
			if (s[i] == ')')
				balance++;
			add_s += s[i++];	
		}
		axioms.push_back(new expression(add_s));
		if (s[i] == ',') {
			i++;	
		}
	}
	if (!axioms.empty()) {
		alpha = axioms.back();
	} else {
		alpha = new expression();
	}
}


bool equal_tree(expression* a, expression* b, map<string, expression*>& m) {
	// fout << a->val << ' ' << b->val << endl;
	if (a->type != b->type && !(a->type == 4 || a->type == 7)) {
		return false;
	}
	if (a->type == 12) {
		return true;
	}
	if (a->type == 7 || a->type == 13) {
		if ((a->name == b->name) && a->arg.size() == b->arg.size()) {
			bool v = true;
			for (int i = 0; i < (int)a->arg.size(); ++i) 
				v &= equal_tree(a->arg[i], b->arg[i], m);
			return v;
		}
		return false;
	}
	if (!(a->type == 4 || a->type == 7)) {
		bool v = equal_tree(a->first, b->first, m);
		if (a->type != 3 && a->type != 11)
			v = v && equal_tree(a->second, b->second, m);
		return v;
	}
	if (m[a->val] == nullptr) {
		m[a->val] = b;
		return true;		
	}
	return exact_equal_tree(m[a->val], b);
}

bool tree_free(expression *y, expression *x) {
	if (y->type == 12) {
		return false;
	}
	if (y->type == 4)
		return *(y) == *(x);
	if (y->type == 5 || y->type == 6) {
		if (*(y->first) == *(x)) {
			return false;
		} else {
			return tree_free(y->second, x);
		}
	}
	if (y->type == 0 || y->type == 1 || y->type == 2 || y->type == 8 || y->type == 9 || y->type == 10) {
		return tree_free(y->first, x) || tree_free(y->second, x);
	}
	if (y->type == 3 || y->type == 11) {
		return tree_free(y->first, x);
	}
	for (expression *e : y->arg) {
		if (tree_free(e, x)) {
			return true;
		}
	}
	return false;
}

bool consist(expression *y, expression *x) {
	if (y->type == 12) {
		return false;
	}
	if (y->type == 4)
		return *(y) == *(x);
	if (y->type == 0 || y->type == 1 || y->type == 2 || y->type == 8 || y->type == 9 || y->type == 10 || y->type == 5 || y->type == 6) {
		return consist(y->first, x) || consist(y->second, x);
	}
	if (y->type == 3 || y->type == 11) {
		return consist(y->first, x);
	}
	for (expression *e : y->arg) {
		if (tree_free(e, x)) {
			return true;
		}
	}
	return false;
}

bool consist_free(expression *b, string& err, int type) {
	expression *x, *y;
	if (type == 3) {
		x = b->second->first;
		y = b->first;
	} else {
		x = b->first->first;
		y = b->second;
	}
	if (tree_free(y, x)) {
		err = ": переменная " + x->val + " входит свободно в формулу " + y->val + ".";
		return true;
	}
	return false;
}

bool tree_with_subst(expression *y, expression *z, expression *x, expression &change, string& err) {
	// fout << y->val << ' ' << z->val << endl;
	if (*(y) == *(x)) {
		if (change.type == -1) {
			change = *(z);
			return true;
		} else {
			return change == *(z);
		}
	} 
	if (y->type != z->type) { 
		return false;
	}
	if (y->type == 12)
		return true;
	if (y->type == 4) {
		return y->val == z->val;
	}
	if (y->type == 0 || y->type == 1 || y->type == 2 || y->type == 8 || y->type == 9 || y->type == 10) {
		return tree_with_subst(y->first, z->first, x, change, err) && tree_with_subst(y->second, z->second, x, change, err);
	}
	if (y->type == 3 || y->type == 11) {
		return tree_with_subst(y->first, z->first, x, change, err);
	}



	if (y->type == 5 || y->type == 6) {
		if (*(y->first) == *(x)) {
			return *(x) == *(z->first) && *(y->second) == *(z->second);
		} else {
			if (*(y->first) == *(z->first) && tree_with_subst(y->second, z->second, x, change, err)) {
				if (change.type != -1) {
					if (consist(&change, y->first)) {
						err = ":";
						return false;
					}
				}
				return true;
			}
			return false;
		}
	}
	if (y->arg.size() == z->arg.size() && y->name == z->name) {
		for (int i = 0; i < (int)y->arg.size(); i++) {
			if (!tree_with_subst(y->arg[i], z->arg[i], x, change, err)) {
				return false;
			}
		}
		return true;
	}
	return false;
}

bool free_to_subst(expression *b, expression &change, string& err, int type) {
	change = expression();
	expression *x = new expression(), *y = new expression(), *y1 = new expression();
	if (b->type == 0 && b->first->type == 5 && type == 10) {
		x = b->first->first;
		y = b->first->second;
		y1 = b->second;
	} 
	if (b->type == 0 && b->second->type == 6 && type == 11) {
		x = b->second->first;
		y = b->second->second;
		y1 = b->first;
	}
	if (x->type != -1) {
		// fout << y->val << ' ' << y1->val << ' ' << x->val << "\n";
		if (!tree_with_subst(y, y1, x, change, err)) {
			if (err == ":") {
				err += " терм " + change.val + " не свободен для подстановки в формулу " + y->val + " вместо переменной " + x->val + ".";
			}
			return false;
		}
		return true;
	}
	return false;
}

bool free_for_ded(expression *b, expression &change, string& err, int type) {
	// fout << b->val << ' ' << type << endl;;
	change = expression();
	if (alpha->type == -1)
		return false;
	expression *x = new expression();
	if (type == 3) {
		x = b->second->first;
	} 
	if (type == 4) {
		x = b->first->first;
	}
	if (tree_free(alpha, x)) {
		err = ": используется ";
		if (type == 3 || type == 4) {
			err += "правило";
		} else {
			err += "схема аксиом";
		}
		err += " с квантором по переменной " + x->val + ", входящей свободно в допущение " + alpha->val + ".";
		return true;
	}
	return false;
}

bool annotation(string s) {
	// fout << "START\n";
	expression *x = new expression(s);
	// bool flag = true;
	// for (auto z: evidence)
	// 	if (*(z) == *(x))
	// 		flag = false;
	// if (flag)
	// 	fout << s << endl;
	evidence.push_back(x);
	string error = "";
	// fout << s << "***\n";

	// fout << "Part of axioms\n";

	for (int i = 0; i < (int)axioms.size(); i++) {
		if (*(axioms[i]) == *(x)) {
			evidence.back()->type_in_proof = 0;
			return true;
		}
	}

	// fout << "Make with MP\n";
	
	for (int i = 0; i < (int)evidence.size() - 1; i++) {
		expression *v = evidence[i];
		if (v->type == 0 && *(v->second) == *(x)) {
			for (int j = 0; j < (int)evidence.size()  -1; j++) {
				if (*(evidence[j]) == *(v->first))  {
					evidence.back()->type_in_proof = 2;
					evidence.back()->mppart = evidence[j];
					return true;
				}
			}
		}
	}

	// fout << "Make with @\n";
//@
	if ((x->type == 0) && (x->second->type == 5)) {
		for (int i = 0; i < (int)evidence.size() - 1; i++) {
			expression *v = evidence[i];
			expression change;
			if (v->type == 0 && *(v->first) == *(x->first) && *(v->second) == *(x->second->second) && !consist_free(x, error, 3) && !free_for_ded(x, change, error, 3)) {
				evidence.back()->type_in_proof = 3;
				evidence.back()->mppart = evidence[i];
				return true;
			}
		}
	}
	// fout << "Make with ?\n";
//?
	if ((x->type == 0) && (x->first->type == 6)) {
		for (int i = 0; i < (int)evidence.size() - 1; i++) {
			expression *v = evidence[i];
			expression change;
			if (v->type == 0 && *(v->second) == *(x->second) && *(v->first) == *(x->first->second) && !consist_free(x, error, 4) && !free_for_ded(x, change, error, 4)) {
				evidence.back()->type_in_proof = 4;
				evidence.back()->mppart = evidence[i];
				return true;
			}
		}
	}  
	// fout << "Scheme of axiom\n";
	for (int i = 0; i < 20; i++) {
		map<string, expression*> m;
		expression change;
		if ((i < 10) && equal_tree(scheme_axiom[i], x, m)) {
			evidence.back()->type_in_proof = 1;
			return true;	
		}	
		if ((i >= 10) && exact_equal_tree(scheme_axiom[i], x, m)) {
			evidence.back()->type_in_proof = 1;
			return true;	
		}
		
		if ((i == 10 || i == 11) && free_to_subst(x, change, error, i)) {
			evidence.back()->type_in_proof = 1;
			return true;	
		}
	}  
	if (x->type == 0 && x->first->type == 2 && x->first->second->type == 5 && x->first->second->second->type == 0) {
		expression *fi = x->second, *kv = x->first->second;
		expression change;
		string err;
		if (tree_with_subst(fi, x->first->first, kv->first, change, err) && change.type == 12 && *(kv->second->first) == *(fi)) {
			change = expression();
			if (tree_with_subst(fi, kv->second->second, kv->first, change, err) && change.type == 11 && *(change.first) == *(kv->first) && !consist(fi, x)) {
				evidence.back()->type_in_proof = 1;
				return true;
			}
		}	
	}

	string a = to_string_int((int)evidence.size());
	s_annotation = "Вывод некорректен начиная с формулы номер " + a + error;
	return false;
}

void change_proof(expression* x) {
	if (alpha->type == -1) {
		fout << to_string(x->val) << "\n";
		return;
	}
	if (x->type_in_proof <= 1 && !(*(x) == *(alpha))) {
		fout << x->val <<"\n(" << x->val << ")->(" << alpha->val << ")->(" << x->val << ")\n(" << alpha->val << ")->(" << x->val << ")\n";
		return;
	}
	if (*(x) == *(alpha)) {
		string a = "(" + alpha->val + ")", aa = a + "->" + a;
		fout << a << "->(" << aa << ")\n";
		fout << "(" << a << "->(" << aa << "))->(" << a << "->((" << aa << ")->" << a << "))->(" << aa << ")\n";
		fout << "(" << a << "->((" << aa << ")->" << a << "))->(" << aa << ")\n";
		fout << "(" << a << "->((" << aa << ")->" << a << "))\n";
		fout << aa << "\n";
		return;
	}
	if (x->type_in_proof == 2) {
		string a = "(" + alpha->val + ")", aj = a + "->(" + x->mppart->val + ")", ai = a + "->(" + x->val + ")", ji = "(" + x->mppart->val + ")->(" + x->val + ")";
		fout << "(" << aj << ")->((" << a << "->(" << ji << "))->(" << ai << "))\n";
		fout << "((" << a << "->(" << ji << "))->(" << ai << "))\n";
		fout << ai << "\n";
		return;
	}
	if (x->type_in_proof == 3) {
		fin = ifstream("lemma1.in");
		string s, a = "(" + alpha->val + ")", b = "(" + x->first->val + ")", c = "(" + x->mppart->second->val + ")";
		while (getline(fin, s)) {
			for (char ch : s) {
				if (ch == 'a')
					fout << a;
				if (ch == 'b')
					fout << b;
				if (ch == 'c')
					fout << c;
				if (ch!= 'a' && ch!='b' && ch!= 'c')
					fout << ch;
			}	
			fout << "\n";		
		}
		string ij = "(" + x->mppart->val + ")", ai = "(" + a + "&(" + x->first->val + "))", j = "(" + x->mppart->second->val + ")";
		//fout << "(" << a << "->" << ij << ")->(" << ai << "->" << j << ")\n";
		fout << "(" << ai << "->" << j << ")\n";
		c = x->second->val;
		fout << ai << "->(" << c << ")\n";
		fin = ifstream("lemma2.in");
		while (getline(fin, s)) {
			for (char ch : s) {
				if (ch == 'a')
					fout << a;
				if (ch == 'b')
					fout << b;
				if (ch == 'c')
					fout << c;
				if (ch!= 'a' && ch!='b' && ch!= 'c')
					fout << ch;
			}	
			fout << "\n";		
		}
		//fout << "(" << ai << "->(" << x->second->val << "))->(" << a << "->" << x->val << ")\n";
		fout << a << "->" << x->val << "\n";
		return;
	}
	fin = ifstream("lemma3.in");
	string s, a = "(" + alpha->val + ")", b = "(" + x->first->val + ")", c = "(" + x->mppart->second->val + ")";
	string ij = "(" + x->mppart->val + ")", aj = "(" + a + "->(" + x->second->val + "))", i = "(" + x->mppart->first->val + ")";  
	while (getline(fin, s)) {
		for (char ch : s) {
			if (ch == 'a')
				fout << a;
			if (ch == 'b')
				fout << b;
			if (ch == 'c')
				fout << c;
			if (ch!= 'a' && ch!='b' && ch!= 'c')
				fout << ch;
		}	
		fout << "\n";		
	}
	fout << i << "->" << aj << "\n";
	b = a;
	a = x->first->val;
	fout << "(" <<  a << ")->" << aj << "\n";
	fin = ifstream("lemma3.in");
	while (getline(fin, s)) {
		for (char ch : s) {
			if (ch == 'a')
				fout << a;
			if (ch == 'b')
				fout << b;
			if (ch == 'c')
				fout << c;
			if (ch!= 'a' && ch!='b' && ch!= 'c')
				fout << ch;
		}
		fout << "\n";
	}
	fout << b << "->" << x->val << "\n";
}

int main() {
	string head, s;
	getline(fin, head);
	axiom_parse(head);
	string arr[21] = {"a->b->a", "(a->b)->(a->b->c)->(a->c)", "a->b->a&b", "a&b->a", "a&b->b", "a->a|b", "b->a|b", "(a->c)->(b->c)->(a|b->c)", "(a->b)->(a->!b)->!a", "!!a->a", "@xY->Y", "Y->?xY",
						"a=b->a'=b'", "a=b->a=c->b=c", "a'=b'->a=b", "!a'=0", "a+b'=(a+b)'", "a+0=a", "a*0=0", "a*b'=a*b+a", "Y&@x(Y->Y)->Y"}; 
	
	for (int i = 0; i < 21; i++) {
		scheme_axiom.push_back(new expression(arr[i]));
	}
	while (getline(fin, s)) {
		// fout << s << endl;
		if (!annotation(s)) {	
			fout << s_annotation << "\n";
			return 0;
		}
	}
	
	for (expression* x : evidence) {
		change_proof(x);
	}	                                                     
	return 0;	
}