#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

string pattern;
unsigned long long patternsize;
map<char, int> bad_char; 
map<int, int> suffixses;
vector<int> good_suffix;
int words;

//Pattern pattern;
string text;
string maked_text;

unsigned long long pattsize;
unsigned long long textsize;
unsigned long long alltextsize;
    


pair<int, int> tmp;

int char_before_all;

int mv1; // moving leaders
int moves; // sdvig posle bm
int prevc; // in reading text



int main(){
    //Pattern

    patternsize = 0;
    words = 0;
    int prevc = ' ';

    int c;
    while( (c = getchar()) != '\n') {
        c = tolower(c);
        cout << c << endl;
        if (c == ' ' || c == '\t') {
            words++;

            if (prevc == ' ' || prevc == '\t') {
                words--;
            } else {
                pattern += c;
                patternsize++;
            }
        } else {
            pattern += c;
            patternsize++;
        }

        prevc = c;
        
    }

    int i;
    //cout << pattern[patternsize-1] << endl;
    if (prevc != ' ' && prevc != '\t') {
        words++;
    } else {
        
        for (i = patternsize-1; i >= 0 && (pattern[i] == ' ' || pattern[i] == '\t'); i--);
        i++;
        pattern.erase(i, patternsize - i);
        patternsize--;
    }
    
    

    if (patternsize != 0){
        // bad_char

        for (int i = 0; i < patternsize - 1; i++) {
            bad_char[pattern[i]] = patternsize - i - 1;
        }
 // suffixes
        int f, g, i;
        
        suffixses[patternsize - 1] = patternsize;
        g = patternsize - 1;
        for (i = patternsize - 2; i >= 0; --i) {
            if (i > g && suffixses[i + patternsize - 1 - f] < i - g)
                suffixses[i] = suffixses[i + patternsize - 1 - f];
            else {
                if (i < g)
                    g = i;
                f = i;
                while (g >= 0 && pattern[g] == pattern[g + patternsize - 1 - f])
                    --g;
                suffixses[i] = f - g;
            }
        }
// good suffix
        good_suffix.resize(patternsize);
        if (pattsize > 1) {
            int j;
            i = 0;
            good_suffix.resize(patternsize, 0);
                
            for (i = 0; i < patternsize; ++i)
                good_suffix[i] = patternsize;
            j = 0;
            for (i = patternsize - 1; i >= 0; --i)
                if (suffixses[i] == i + 1)
                    for (; j < patternsize - 1 - i; ++j)
                        if (good_suffix[j] == patternsize)
                            good_suffix[j] = patternsize - 1 - i;
            for (i = 0; i <= patternsize - 2; ++i)
                good_suffix[patternsize - 1 - suffixses[i]] = patternsize - 1 - i;
        }
        
    }
// bm.cpp
    pattsize = patternsize;
    
    textsize = 0;
    alltextsize = 0;
    text = "";
    maked_text = "";
    tmp.first = 1;
    tmp.second = 1;

    char_before_all = ' ';
    prevc = ' ';

// work

    //cout << "pattern: " << pattern.pattern << "!" << endl;
       
    //read_text();


    c = ' ';
    if (text == "") {
        prevc = ' ';
    }else {
       prevc = text[alltextsize-1];
    }
        
    while (textsize < pattsize) {    
        c = getchar();
        if (c == EOF) break;
        c = tolower(c);
        
        if (c == ' ' || c == '\t' || c == '\n') {
            if (prevc == ' ' || prevc == '\t' || prevc == '\n') {
                textsize--;
            }
        }

        text += c;
        textsize++;
        alltextsize++;
            
        prevc = c;
            
    }
        //cout << "textsize: " << textsize << endl;
    //cout << "text: " << text << "!" << endl;

    do {


        //prepare_text();

        //finding first not ' '
        for(mv1 = 0; text[mv1] == ' ' || text[mv1] == '\n' || text[mv1] == '\t'; mv1++);
        //cout << "to delete in front: " << mv1 << endl;
        
        // changing tmp for all ' '
        for (int j = 0; j < mv1; j++){
            if (text[j] == '\n') {
                tmp.first++;
                tmp.second = 1;
            }
            if (text[j] == ' ' ||text[j] == '\t') {
                if (char_before_all != ' ' && char_before_all != '\t' && char_before_all != '\n') {
                    tmp.second++;
                }
            }

            char_before_all = text[j]; 
        }

        text.erase(0, mv1);
        
    
        
        alltextsize -= mv1;

        ///cout << "prepared: " << text << "!" <<  endl;


        //make_text();

        maked_text = "";
        int prevchar = ' ';
        for(int i = 0; i < text.length(); i++) {
            if (text[i] == ' ' || text[i] == '\t' || text[i] == '\n') {
                if (prevchar != ' ' && prevchar != '\t') {
                    if (text[i] == '\n'){
                        maked_text += ' ';
                    } else {
                        maked_text += text[i];
                    }
                }
            } else {
                maked_text += text[i];
            }
            if (text[i] == '\n'){
                prevchar = ' ';
            } else {
                prevchar = text[i];
            }
        }

        //cout << "maked: " << maked_text << "!" << endl;
        //cout << "patte: " << pattern.pattern << "!" << endl;


        //compare();
        
        int i;
        //cout << pattern.pattern[i] << "#" << text[i-1] << endl;
        for (i = pattsize-1; i >=0 && pattern[i] == maked_text[i]; i--){
            //cout << pattern.pattern[i] << "#" << maked_text[i] << endl;
        }
         
        if (i < 0) {
            cout << tmp.first << ", " << tmp.second << endl;
            moves = good_suffix[0];

        } else {
            map<char,int>::iterator it;
            it = bad_char.find(text[i]);
            if (it == bad_char.end())  {
                moves = pattsize-1;
            } else {
                moves = it->second;
            }
            moves = max(good_suffix[i], (int)(moves - pattsize + i + 1));
        }
        
        //moving();
        
        i = 0; // kol-vo to move
        int j = 0; // position

        while (i < moves) {
            if (text[j] == '\n') {
                tmp.first++;
                tmp.second = 1;
            }

            if (text[j] == ' ' || text[j] == '\t' || text[j] == '\n') {
                if (char_before_all == ' ' || char_before_all == '\t' || char_before_all == '\n') {
                    i--;
                } else {
                    tmp.second++;
                }
            }
            char_before_all = text[j];
            j++;
            i++;
        }

       // cout << "j: " << j << endl;
        alltextsize -= j;
        textsize -= moves;

        text.erase(0, j);
        
        
        
        //cout << "text move: " << text << "!" << endl;
        //cout << "textsize: " << textsize << " alltextsize: " << alltextsize << endl;
        //read_text();
        
        int c = ' ';
        if (text == "") {
            prevc = ' ';
        } else {
            prevc = text[alltextsize-1];
        }
        
        while (textsize < pattsize) {
            
            c = getchar();
            if (c == EOF) break;
            c = tolower(c);
            
            if (c == ' ' || c == '\t' || c == '\n') {
                if (prevc == ' ' || prevc == '\t' || prevc == '\n') {
                    textsize--;
                }
            }

            text += c;
            textsize++;
            alltextsize++;
            
            prevc = c;
            
        }
        
        
        
        //cout << "text read: " << text << "!" << endl;
        //cout << "textsize: " << textsize << " alltextsize: " << alltextsize << endl;

    } while (textsize == pattsize);


} // main
