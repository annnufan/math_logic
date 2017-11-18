#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string base = "";
string proof = "";
string proof2 = "";
string morgan = "";
string lemm = "";

string text_of_proof = "";

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

void init() {
	string s;
	ifstream in1("base.in");
	while (getline(in1, s)) {
		base += s + "\n";		
	}
	in1.close();
	ifstream in2("proof.in");
	while (getline(in2, s)) {
		proof += s + "\n";		
	}
	in2.close();
	ifstream in3("proof2.in");
	while (getline(in3, s)) {
		proof2 += s + "\n";		
	}
	in3.close();
	ifstream in4("Morgan.in");
	while (getline(in4, s)) {
		morgan += s + "\n";		
	}
	in4.close();
	ifstream in5("lemm for a'+b.in");
	while (getline(in5, s)) {
		lemm += s + "\n";		
	}
	in5.close();
}

void solve(int a, int b) {
	text_of_proof = "";
	int c = b - a;
	string s;
	text_of_proof += base;
	if (c >= 0) {
		string A = "0", B = "0", C = "0";
		for (int i = 0; i < b; i++) {
			if (i < c)
				C += "'";
			if (i < a)
				A += "'";
			B += "'";
		}
		text_of_proof += "@a(a+0=a)->" + A + "+0=" + A + "\n";
		text_of_proof += A + "+0=" + A + "\n";
		string my_proof = "";
		for (char ch:proof) {
			if (ch == 'A') {
				my_proof += A;
			} else {
				my_proof += ch;
			}
		}
		string sh = "";
		for (int i = 0; i < c; i++) {
			for (char ch:my_proof) {
				if (ch == '#') {
					text_of_proof += sh;
				} else {
					text_of_proof += ch;
				}
			}
			sh += "'";
		}
		text_of_proof += "(" + A + "+" + C + ")=" + B + "->?p(" + A + "+p)=" + B + "\n";
		text_of_proof += "?p(" + A + "+p)=" + B + "\n";
	} else {
		text_of_proof += lemm;
		//C0 = C-1
		c = -c;
		string C0 = "", B = "";
		for (int i = 0; i < c - 1; i++) {
			if (i < b)
				B += "'";
			C0 += "'";
		}
		if (c - 1 < b) {
			for (int i = c - 1; i < b; i++) {
				B += "'";
			}
		}
		s = "";
		for (char ch:proof2) {
			if (ch == 'C') {
				text_of_proof += C0;
			} else {
				if (ch == 'B'){
					text_of_proof += B;
				} else
					text_of_proof += ch;
			}
		}
		for (char ch:morgan) {
			if (ch == 'C') {
				text_of_proof += C0;
			} else {
				if (ch == 'B'){
					text_of_proof += B;
				} else
					text_of_proof += ch;
			}
		}
	}
}

int main() {
	init();
	for (int i = 0; i <= 100; i++) {
		for (int j = 0; j <= 100; j++) {
			if (i > j){
				string s = "";
				if (i <100)
					s += "0";
				if (i < 10)
					s+= "0";
				s += to_string_int(i);
				if (j <100)
					s += "0";
				if (j < 10)
					s+= "0";
				s += to_string_int(j);
				ofstream fout(s);
				solve(i, j);
				fout << text_of_proof;
			}
		}
	}
	return 0;
}