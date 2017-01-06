#include <iostream>
#include <fstream>
#include <string>

using namespace std;

ifstream base("base.in");
ifstream proof("proof.in");
ifstream proof2("proof2.in");
ifstream morgan("Morgan.in");
ifstream lemm("lemm for a'+b.in");

ifstream fin("in.in");
ofstream fout("out.out");


int main() {
	int a, b, c;
	fin >> a >> b;
	c = b - a;
	string s;
	while (getline(base, s))
		fout << s << "\n";
	if (c >= 0) {
		string A = "0", B = "0", C = "0";
		for (int i = 0; i < b; i++) {
			if (i < c)
				C += "'";
			if (i < a)
				A += "'";
			B += "'";
		}
		fout << "@a(a+0=a)->" << A << "+0=" << A << "\n";
		fout << A << "+0=" << A << "\n";
		string my_proof = "";
		while (getline(proof, s)) {
			for (char ch:s) {
				if (ch == 'A') {
					my_proof += A;
				} else {
					my_proof += ch;
				}
			}
			my_proof += "\n";
		}
		string sh = "";
		for (int i = 0; i < c; i++) {
			string p = "";
			for (char ch:my_proof) {
				if (ch == '#') {
					p += sh;
				} else {
					p += ch;
				}
			}
			my_proof = p;
			fout << my_proof;
			sh += "'";
		}
		fout << "(" << A << "+" << C << ")=" << B << "->?p(" << A << "+p)=" << B << "\n";
		fout << "?p(" << A << "+p)=" << B << "\n";
	} else {
		while (getline(lemm, s))
			fout << s << "\n";
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
		while (getline(proof2, s)) {
			for (char ch:s) {
				if (ch == 'C') {
					fout << C0;
				} else {
					if (ch == 'B'){
						fout << B;
					} else
						fout << ch;
				}
			}
			fout << "\n";
		}
		while (getline(morgan, s)) {
			for (char ch:s) {
				if (ch == 'C') {
					fout << C0;
				} else {
					if (ch == 'B'){
						fout << B;
					} else
						fout << ch;
				}
			}
			fout << "\n";
		}
	}
	return 0;
}
	