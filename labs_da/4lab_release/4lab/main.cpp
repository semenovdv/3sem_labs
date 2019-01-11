#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

string getthestring() {
    string temp = "";
    int c;
    while( (c = getchar()) != '\n') {
        c = tolower(c);
        temp += c;
    }
    
    return temp;
}


vector<unsigned long long> good_suff(string &s) {
	unsigned long len = s.length();

	vector<unsigned long long> gs(len + 1, 0);
	vector<unsigned long long> f(len+1);

	f[len] = len + 1;
	unsigned long long j = len + 1;

	for (unsigned long long i = len; i > 0; i--) {
		while (j <= len && s[i-1] != s[j-1]) {
			if (gs[j] == 0) gs[j] = j-i;
			j = f[j];
 		}
		
		f[i-1] = --j;
	}
	unsigned long long p = f[0];
	for (j = 0; j <= len; j++){
		if (gs[j] == 0) gs[j] = p;
		if (j == p) p = f[p];
	}

	return gs;
}

vector<unsigned long long> bad_char(string &s) {
	vector<unsigned long long> bc(128);

	unsigned long long len = s.size();
	for (int i = 0; i < 128; i++) bc[i] = len;

	for (unsigned long long i = 0; i < len-1; i++) {
		bc[s[i]] = len - i - 1;
	}

	return bc;
}

long long bm_find( string &pat, string &tex_s, vector<unsigned long long> &bc, vector<unsigned long long> &gs, long long wpos, unsigned long long lc) {

	long long j;
	string tex;

	for(unsigned long long k = 0; k < tex_s.length(); k++) {
		if (tex_s[k] == '\n') {
			tex += ' ';
		} else {
			tex += tex_s[k];
		}
	}

	//cout << "check: " << tex << endl;

	unsigned long long len = pat.length();
	long long res = 0;

	for (j = len - 1; j >= 0 && pat[j] == tex[j]; j-- ); // сдвиг, пока можем

	if (j < 0) {
		// no bc
		cout << lc << ", " << wpos << endl;
		res = gs[j+1];
	} else {
		// yes bc
		/////!!!!!! нет проверки на выход за границы
		if (gs[j+1] > bc[tex[j]] - len + j + 1) {
			//cout << "gs" << endl;
			res = gs[j+1];
		} else {
			//cout << "bc" << endl;
			//cout << "bc[tex[j]]: " << bc[tex[j]] << endl;
			//cout << "len: " << len << endl;
			//cout << "j: " << j << endl;
			res = bc[tex[j]] - len + j + 1;
			if (res < 0) {
				res = 1;
			}
			//cout << "res: " << res << endl;
			
		}
	}
		
	return res;

}

int main() {
    string templat = getthestring();
	
	vector<unsigned long long> bc = bad_char(templat);
	
	//cout << "ok bc" << endl;
	/*
	for( int i = 0; i < bc.size(); i++){
		cout << i << ": " << bc[i] << endl;;
	}
	cout << endl;
	cout << endl;
	*/
	vector<unsigned long long> gs = good_suff(templat);
	/*
	cout << "oj" << endl;
	for( int i = 0; i < gs.size(); i++){
		cout << gs[i] << " ";
	}
	cout << endl;
	*/

	string text = "";
	unsigned long lines_count = 1;
	unsigned long long char_counter = 0;
	long long word_counter = 1;

	char char_before = ' ';

	
	do {
		
		while (char_counter < templat.length()) {
			int c = getchar();
			if (c == EOF) exit(0);
				
			char_counter++;
			c = tolower((unsigned long long)c);
        	text += c;
			
		}	

		
		
		unsigned long long shift = bm_find(templat, text, bc, gs, word_counter, lines_count);
		//cout << "shift: " << shift << endl;
		
		char_counter = char_counter - shift;
	
		for (unsigned long long k = 0; k < shift; k++) {
			// проверка на новое слово
			if (char_before == ' ' || char_before == '\n') {
				if (text[k] == ' ' || text[k] == '\n')  word_counter--;
			}

			if (text[k] == ' ' || text[k] == '\n')  word_counter++;

			char_before = text[k];
			
			// проверка на новую строку
			if (text[k] == '\n') {
				lines_count++;
				word_counter = 1;

			}
		}
		
		
		for( unsigned long long k = 0; k < templat.length() - shift; k++){
			text[k] = text[k + shift];
		}
	
		text.resize(templat.length() - shift);
		

	} while (true);

	

    
    return 0;
}