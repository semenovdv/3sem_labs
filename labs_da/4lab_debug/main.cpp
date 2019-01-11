#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <string>
#include <algorithm>
/*

отрабоать смену строк


*/

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

long long bm_find( string &pat, string &tex, vector<unsigned long long> &bc, vector<unsigned long long> &gs, long long wpos, unsigned long long lc) {

	long long j = 0;

	//cout << "check: " << tex << "!" << tex.length() <<  endl;
	//cout << "patte: " << pat << "!" << pat.length() << endl;

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
			res = gs[j+1];
		} else {
			res = bc[tex[j]] - len + j + 1;
			if (res < 0) {
				res = 1;
			}
			
		}
	}
		
	return res;

}







int main() {
    string templat = getthestring();
	
	vector<unsigned long long> bc = bad_char(templat);
	vector<unsigned long long> gs = good_suff(templat);
	

	string text = "";
	unsigned long lines_count = 1;
	long long char_counter = 0;
	long long word_counter = 1;

	char char_before = ' '; // begin

	int prevc = ' '; // end
	int prev = ' '; // to tex 
	

	do {
		// считывание
		
		while (char_counter < templat.length()) {

			cout << "char_couner: " << char_counter << endl;
			int c = getchar();
			
			if (c == EOF) exit(0);
			if ((c == ' ' || c == '\n') && (prevc == ' ' || prevc == '\n')) {
				char_counter--;
			} 
			c = tolower(c);
			text += c;
			
			prevc = c;
			char_counter++;
		}	
		// конец считывания
		

		string tex = "";
		
		cout << "befor: " << endl << text << "!" << endl;
		for (int i = 0; i < text.length(); i++) {
			cout << "!" << text[i] << "!" << endl;
		}
		
		// затираем передние ненужные в тексте
		int k = 0;
		int shifttofit = 0;
		
		while (text[k] == '\n' || text[k] == ' ') {
			
			if (text[k] == '\n') {
				lines_count++;
				word_counter = 1;
			}
			if (text[k] == ' ' && char_before != ' '){
				word_counter++;
			}
			char_before = text[k];
			shifttofit++;
			k++;
		}

		char_counter = char_counter - shifttofit;

		cout << "lines_count after read: " << lines_count << endl;
		
		
		for (int i = 0; i < text.length() - shifttofit; i++) {
			text[i] = text[i + shifttofit];
		}
		int tlen = text.length();
		for( int i = tlen - shifttofit; i < tlen ; i++) {
			
			//cout << "!" << text << "!" << endl;
			text.pop_back();

		}

		// конец затирания лидирующих 
		
		//cout << "!" << text << "!" << endl;

		//cout << shifttofit << endl;
		cout << "after: " << endl << text << "!" << endl;
		for (int i = 0; i < text.length(); i++) {
			cout << "!" << text[i] << "!" << endl;
		}
		
		// формирование текста на подачу 
		prev = ' ';
		for(unsigned long long k = 0; k < text.length(); k++) {
			
			/*if ((text[k] == ' ' || text[k] == '\n') && (prev == ' ' || prev == '\n')) {
				prev = text[k];
			} else {*/
				if ((text[k] == '\n' || text[k] == ' ') && (prev == ' ' || prev == '\n')) {
					prev = text[k];
					
				} else {
					if (text[k] == '\n') tex += ' ';
					else tex += text[k];
				}
				prev = text[k];
			//}
		}

		// конец формирования текста на подачу
		
		cout << "!!!!!!!!!!!!!!!" << endl;
		for (int i = 0; i < tex.length(); i++) {
			cout << "!" << tex[i] << "!" << templat[i] << "!" << endl;
		}
		cout << "!!!!!!!!!!!!!!!" << endl;
		
		// поиск
		unsigned long long shift = 0;
		if (tex.length() == templat.length()) {

		
			shift = bm_find(templat, tex, bc, gs, word_counter, lines_count);
		
			cout << "shift: " << shift << endl;
			// конец поиска 

			char_counter = char_counter - shift;
			
			// проверка на сдвиги
			//int sdvig = 0;
			tlen = text.length();
			
			int move = 0;
			for (int i = 0; i < tlen - shift; i++) {
				if (text[i+move] == '\n') {
					lines_count++;
					word_counter = 1;
				}
				if ((text[i+move] == ' ' || text[i+move] == '\n')  && (har_before != '\n' && char_before != ' ')) { // TODO ??
					word_counter++;
				}
				char_before = text[i+move];
				text[i+move] = text[i+shift + move];
				//move++;
			}
			cout << "lines_count after moving: " << lines_count << endl;
			cout << "alert: " << text << "!" << endl;

			//cout << "sdvig: "<< sdvig << endl;
			//char_counter = char_counter - sdvig;
			cout << "char_couner: " << char_counter << endl;

			// затирание лидирующих ненужных
			k = 0;
			shifttofit = 0;

			while (text[k] == '\n' || text[k] == ' ') {
			
				if (text[k] == '\n') {
					lines_count++;
					word_counter = 1;
				}
				if (text[k] == ' ' && char_before != ' '){
					word_counter++;
				}
				char_before = text[k];
				shifttofit++;
				k++;
			}

			char_counter = char_counter - shifttofit;
			cout << "lines_count after moving: " << lines_count << endl;
			
			
			for (int i = 0; i < text.length() - shifttofit; i++) {
				text[i] = text[i + shifttofit];
			}
			int tlen = text.length();
			for( int i = tlen - shifttofit; i < tlen ; i++) {
				
				//cout << "!" << text << "!" << endl;
				text.pop_back();

			}


			//char_counter = char_counter - shifttofit;
			//cout << "char_couner: " << char_counter << endl;

			
			cout << "resize : " << text << "!" << endl;
			for (int i = 0; i < text.length(); i++) {
				cout << "!" << text[i] << "!" << endl;
			}
			/*
			cout << "text.length(): " << text.length() << endl;
			cout << "shift + sdvig + shifttofit: " << shift + sdvig + shifttofit << endl;
			
			if (text.length() <= shift + sdvig + shifttofit) {
				text = "";
				char_counter = 0;
			} else {
				text.resize(templat.length() - (shift + sdvig + shifttofit));
			}
			*/
			//cout << "after resize: " << text << "!" << endl;
		}

	} while (true);

	

    
    return 0;
}



/*
// считывание с затиранием пробелов
		while (char_counter < templat.length()) {
			int c = getchar();
			//cout << ">!" << (char)c << "!<" << endl;
			
			if (c == EOF) exit(0);
			if (c == '\n') {
				lines_count++;
				word_counter = 1;
				//char_counter--;
				if (prevc != ' ' && prevc != '\n'){
					text += ' ';
				} else {
					char_counter--;
				}
			} else if ((c == ' ') && (prevc == ' ' || prevc == '\n')) {
				char_counter--;
			} else {
				c = tolower(c);
				text += c;
			}
			
			prevc = c;
			char_counter++;
		}	

*/