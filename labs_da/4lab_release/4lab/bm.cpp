#include "Pattern.h"
using namespace std;

/*

    all works

    make_text quite slow ~ 70% of time
    how may be: 
        make a queue with pairs (nstring - nword) and remake text while reading it
        have to see how it works at github ---
        ---DONEDONEDONE---

        valgrind pos.push(tmp) and pos.pop()   invalid write and uninitialized
        and pos.size()

        adding pattern to end of 1st abzatc, all woks

*/


class BM {
public:
    BM(){
        pattsize = pattern.patternsize;
        words = pattern.words + 1;
        textsize = 0;
        text = "";
        maked_text = "";
        tmp.first = 1;
        tmp.second = 1;
        char_before_all = ' ';
        prevc = ' ';

    }

    

    void read_text() {
        int c = ' ';
        
        while (textsize < pattsize) {
            
            c = getchar();
            if (c == EOF) break;
            c = tolower(c);
            //cout << "got: !" << (char)c << "!" << endl;
            
            if (c == ' ' || c == '\t') {
                if (!(prevc == ' ' || prevc == '\t' || prevc == '\n')) {
                    if (c == '\t') c = ' ';
                    text += c;
                    ++tmp.second;
                } else {
                    textsize--;
                }
            } else if (c == '\n') {
                if (!(prevc == ' ' || prevc == '\t' || prevc == '\n')) {
                    text += ' ';
                } else {
                    textsize--;
                }
                ++tmp.first;
                tmp.second = 1;
                
            } else { // DONE
                
                if (prevc == ' ' || prevc == '\n' || prevc == '\t') {

                    if (pos.size() == words + 1) {
                        //cout << "deleteng: " << pos.front().first << " " << pos.front().second << endl;
                        //pos.pop();
                        
                    }
                    pos.push(tmp);
                    //cout << "pushing tmp:" << tmp.first << " " << tmp.second << endl;
                } 
                
                text += c;  
                
            }

            
            textsize++;
            
            prevc = c;
            
        }
        //cout << "textsize: " << textsize << endl;        
    }

    // deleting all in front to work normally
    void prepare_text() {
        
        if (text[0] == ' '){
            if (char_before_all != ' ') {
                //cout << "deleteng: " << pos.front().first << " " << pos.front().second << endl;
                pos.pop();
            }
            text.erase(0, 1);
            textsize--;
            
        }
        
    }


    void compare(){
        int i;
        
        for (i = pattsize-1; i >=0 && pattern.pattern[i] == text[i]; i--);
         
        if (i < 0) {
            //if (pos.empty()) cout << "ALERT POS IS EMPTY" << endl;
            //cout << pos.size() << endl;
            //cout << pos.front().first << endl;
            pair<int, int> mytmp = pos.front();
            
            cout << mytmp.first << ", " << mytmp.second << endl;
            move = pattern.good_suffix[0];
            //cout << "move1: " << move << endl;

        } else {
            map<char,int>::iterator it;
            it = pattern.bad_char.find(text[i]);
            if (it == pattern.bad_char.end())  {
                move = pattsize-1;
                //cout << "move2: " << move << endl;
            } else {
                move = it->second;
                //cout << "move3: " << move << endl;
            }
/*

    CHANGING "I" TO "I+1" IN GOOD SUFFIX

*/



            if (pattern.good_suffix[i+1] > (int)(move)) {
                move = pattern.good_suffix[i];
                //cout << "move4: " << move << endl;
                //cout << "moving good suffix: " << pattern.good_suffix[i] << endl;
            } else if (pattern.good_suffix[i+1] <= (int)(move)) {
                //move = (int)(move - pattsize + i + 1);
                //cout << "move5: " << move << endl;
                //cout << "moving bc (bc[text[i]] - pattsize + i + 1): " << (int)(move - pattsize + i + 1) << endl;
            } else {
                //qmove = pattern.good_suffix[i];
                //cout << "move6: " << move << endl;
                //cout << "equals: " << move << endl;
            }
            /*
            if (pattern.good_suffix[i] > (int)(move - pattsize + i + 1)) {
                move = pattern.good_suffix[i];
                cout << "move4: " << move << endl;
                //cout << "moving good suffix: " << pattern.good_suffix[i] << endl;
            } else if (pattern.good_suffix[i] > (int)(move - pattsize + i + 1)) {
                move = (int)(move - pattsize + i + 1);
                cout << "move5: " << move << endl;
                //cout << "moving bc (bc[text[i]] - pattsize + i + 1): " << (int)(move - pattsize + i + 1) << endl;
            } else {
                move = pattern.good_suffix[i];
                cout << "move6: " << move << endl;
                //cout << "equals: " << move << endl;
            }
            */
            //move = max(pattern.good_suffix[i], (int)(move - pattsize + i + 1));
            
        }

        //cout << "move: " << move << endl; 
        
    }

    void moving() {
        //cout << "move: " << move << endl;
        int i = 0; // kol-vo to move

        move_char = '`';

        while (i < move) {
            int ch = text[i];
            if (ch == ' ' || ch == '\t' || ch == '\n'){
                //cout << "deleteng: " << pos.front().first << " " << pos.front().second << endl;
                pos.pop();
            }
            char_before_all = text[i];
            i++;
        }
        move_char = char_before_all;
        //cout << "now char_before_all=" << (char)char_before_all << endl;

       // cout << "j: " << j << endl;
        //alltextsize -= j;
        textsize -= move;

        text.erase(0, move);
        //cout << "first after moving:" << pos.front().first << " " << pos.front().second << endl;
        
    }

    void work(){
        cout << "pattern: " << pattern.pattern << "!" << endl;
       
        read_text();
        cout << "textsize: " << textsize << endl;
        cout << "text: " << text << "!" << endl;

        do {
            cout << "DOOOOOOOOOOOOOOOO" << endl;
            prepare_text();
            cout << "prepared: " << text << "!" <<  endl;
            cout << "textsize n pattsize: " << textsize << " " << pattsize << endl;
            
            if (textsize < pattsize) {
                cout << "not enough prepared" << endl;
                read_text();
                prepare_text();
                cout << "read n prepared: " << text << endl; 
            }
            //cout << "prepared: " << text << "!" <<  endl;
            //make_text();
            //if (tmp.first == 37) {
                //cout << "oops" << endl;
            //}
            //cout << "maked: " << maked_text << "!" << endl;
            cout << "patte: " << pattern.pattern << "!" << endl;
            
            compare();
            moving();
            cout << "text move: " << text << "!" << endl;
            //cout << "textsize: " << textsize << " alltextsize: " << alltextsize << endl;
            prepare_text(); // CHANGED
            cout << "text moved/prepared: " << text << "!" << endl;
            //cout << "textsize: " << textsize << " alltextsize: " << alltextsize << endl;
            read_text();
            cout << "text read: " << text << "!" << endl;
            //cout << "textsize: " << textsize << " alltextsize: " << alltextsize << endl;

        } while (textsize == pattsize);
    
    }

    void work2(){
       
        read_text();
        

        do {
            prepare_text();
            
            if (textsize < pattsize) {
                read_text();
                //prepare_text();
            }
            //make_text();
            
            compare();
            moving();
            
            prepare_text(); // CHANGED
            
            read_text();
            

        } while (textsize == pattsize);
    
    }

private:
    Pattern pattern;
    string text;
    string maked_text;

    unsigned long long pattsize;
    unsigned long long textsize;
    unsigned long long alltextsize;
    
    int words;

    queue<pair<int, int>> pos;
    pair<int, int> tmp;

    int char_before_all;

    int mv1; // moving leaders
    int move; // sdvig posle bm
    int prevc; // in reading text

    int move_char;
 
};

int main(){
    BM obj;

    //obj.work();
    obj.work2();
}