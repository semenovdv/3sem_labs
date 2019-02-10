#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>


using namespace std;


// class is using for managing with teplate and all its operations
class Pattern {
public:
    // reading pattern and preprocessing
    Pattern(){ 
        patternsize = 0;

        int c;
        while( (c = getchar()) != '\n') {
            c = tolower(c);
            pattern += c;
            patternsize++;
        }


        make_bad_char();
        make_good_suffix();
       
    }

    void Print_pattern() {
        cout << "pattern: " << endl;
        for (size_t i = 0; i < patternsize; i++) {
            cout << pattern[i] << " ";
        }
        cout << endl;
    }

    void Print_bad_char() {
        cout << "bad char: " << endl;
        for (auto &i : bad_char) {
            cout << i.first << " " << i.second << endl;
        }
        cout << endl;
    }

    void Print_good_suffix() {
        cout << "suffxes: " << endl;
        for (int i = 0; i < good_suffix.size(); i++) {
            cout << good_suffix[i] << endl;
        }
        cout << endl;
    }

    void Print() {
        Print_pattern();
        Print_bad_char();
        Print_good_suffix();
    }

    ~Pattern(){
       
    }

private:

    // careful! if not found, then templatelen
    void make_bad_char(){
        for (int i = 0; i < patternsize - 1; i++) {
            bad_char[pattern[i]] = patternsize - i - 1;
        }
    }

    bool suffix_is_prefix(int pos) {
        int i = 0;
        int suffixlen = patternsize - pos;

        for (i = 0; i < suffixlen; i++) {
            if ( pattern[i] != pattern[pos + i]) 
                return false;
        }

        return true;
    }

    int suffix_len(int pos) {
        int i;

        for (i = 0; (pattern[pos - 1] == pattern[patternsize - 1 - i]) && 
        (i < pos); i++);
        return i;
        
    }
/*
    // may think it works
    // this is N func
    void make_suffixes() { 
        suffixses.resize(patternsize);
        suffixses[patternsize - 1] = 0;
        int k = 0;

        for (int i = patternsize - 2; i >= 0; i--) {
            while ((k > 0) && pattern[patternsize - 1 -k] != pattern[i])
                k = suffixses[k-1];
            if (pattern[patternsize -1 -k] == pattern[i])
                k++;
            suffixses[i] = k;
        }
        
    }
*/

    void suffixes() {
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
    
    void make_good_suffix() {
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
        for (i = 0; i <= patternsize - 2; ++i)
            good_suffix[patternsize - 1 - suffixses[i]] = patternsize - 1 - i;
    }

    string pattern;
    unsigned long long patternsize;
    map<char, int> bad_char; 
    map<int, int> suffixses; // N func
    vector<int> good_suffix;

};