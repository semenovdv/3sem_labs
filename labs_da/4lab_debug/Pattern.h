#ifndef PATTERN_H
#define PATTERN_H

#include "maininc.h"

using namespace std;


class Pattern {
public:
    Pattern();
    void Print();
    void Print_pattern();
    void Print_bad_char();
    void Print_good_suffix();
    virtual  ~Pattern();


    void make_bad_char();
    void suffixes();
    void make_good_suffix();

    string pattern;
    unsigned long long patternsize;
    map<char, int> bad_char; 
    map<int, int> suffixses;
    vector<int> good_suffix;
    int words;

};

// HERE
// READING AND PREPROCESSING
Pattern::Pattern() {
    patternsize = 0;
    words = 0;
    int prevc = ' ';

    int c;
    while( (c = getchar()) != '\n') {
        c = tolower(c);
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
        make_bad_char();
        make_good_suffix();
    }
    
    
    //cout << pattern <<  " " << patternsize << endl;
    //cout << "words: " << words << endl;
    
}

 
void Pattern::Print() {
    Print_pattern();
    Print_bad_char();
    Print_good_suffix();
}


void Pattern::Print_pattern() {
    cout << "pattern: " << endl;
    for (size_t i = 0; i < patternsize; i++) {
        cout << pattern[i] << " ";
    }
    cout << endl;
}

void Pattern::Print_bad_char() {
    cout << "bad char: " << endl;
    for (auto &i : bad_char) {
        cout << i.first << " " << i.second << endl;
    }
    cout << endl;
}

void Pattern::Print_good_suffix() {
    cout << "suffxes: " << endl;
    for (int i = 0; i < good_suffix.size(); i++) {
        cout << good_suffix[i] << endl;
    }
    cout << endl;
}

void Pattern::make_bad_char(){
    for (int i = 0; i < patternsize - 1; i++) {
        bad_char[pattern[i]] = patternsize - i - 1;
    }
}

void Pattern::suffixes() {
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
}

void Pattern::make_good_suffix() {
    int i, j;
    good_suffix.resize(patternsize);
        
    suffixes();
        
    for (i = 0; i < patternsize; ++i)
        good_suffix[i] = patternsize;
    j = 0;
    
    for (i = patternsize - 1; i >= 0; --i)
        if (suffixses[i] == i + 1)
            for (; j < patternsize - 1 - i; ++j)
                if (good_suffix[j] == patternsize)
                    good_suffix[j] = patternsize - 1 - i;
    
    if (patternsize > 1) 
        for (i = 0; i <= patternsize - 2; ++i)
            good_suffix[patternsize - 1 - suffixses[i]] = patternsize - 1 - i;
}

Pattern::~Pattern(){

}

#endif // PATTERN_H
