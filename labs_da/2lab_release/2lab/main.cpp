#include "TRBTree.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include <cstring>


using std::cout;
using std::cin;
using std::string;
using std::endl;

bool isletter( char ch ) {
    return ( ch >= 'a' && ch <= 'z' ) || ( ch >= 'A' && ch <= 'Z' );
}
bool isletter2( char ch ) {
    return ( ch >= 'a' && ch <= 'z' ) || ( ch >= 'A' && ch <= 'Z' ) || (ch == '/') || (ch == '-');
}

void GetKey( char key[] ) {

    char ch;
    int idx = 0;


    while ( true ) {
        ch = tolower( getchar());

        if ( !isletter2( ch )) {
            break;
        }

        key[idx++] = ch;
    }
    key[idx] = '\0';
}

void GetVal( unsigned long long &val ) {
    char ch;
    val = 0;
    while (( ch = getchar()) != '\n' && ch != ' ') {
        val = val * 10 + ch - '0';
    }

}

typedef struct {
	bool is_list;
    nodeColor color; 
    unsigned long long m_value;
    char m_key[260];
} PrMin;

int main() {
    std::ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    char save[5] = "save";
    char load[5] = "load";
    //char print[6] = "print";

    

    TRBTree tr;

    while (true){

        char op = '\n';

        while(op == '\n' || op == ' '){
            op = tolower(getchar());
        }

        if( op == EOF){
            return 0;
        }
        
        if (op == '+') {
            getchar();
            
            unsigned long long v = 0;;
            char *k = (char*)malloc(sizeof(char) * 260);//[260];

            for(int i = 0; i < 260; i++){
                k[i] = '\0';
            }
            GetKey(k);
            GetVal(v);
            try{
                tr.Insert_kv(k, v);
            } catch(std::bad_exception&) {
                cout << "ERROR: Not enough memory" << endl;
                tr.Clear_Tree(tr.GetRoot());
                return 0;
            }
            
            free(k);
        } else if (op == '-') {
            getchar();
        
                char k[260];
                for(int i = 0; i < 260; i++){
                    k[i] = '\0';
                }
                GetKey(k);
                tr.Delete_k(k);
            
        } else if (op == '!'){
            getchar();
            
                char *action = (char*)malloc(sizeof(char) * 260);
                GetKey(action);

                if (strcmp(action, save) == 0) {
                    char path[260] = {'\0'};
                    GetKey(path);

                    std::ofstream outFile(path, std::ios::binary | std::ios::out);

                    if (! outFile.is_open()) {
                        cout << "ERROR: Couldn`t create file" << endl;
                        
                    } else {
                        tr.Save_Tree(&outFile, tr.GetRoot());
                        outFile.close();
                        cout << "OK" << endl;
                    }

                } else if (strcmp(action, load) == 0) {
                    char path[260] = {'\0'};
                   GetKey(path);
                    std::ifstream inFile(path, std::ios::binary | std::ios::in);

                    if (! inFile.is_open()) {
                        cout << "ERROR: Unable to open file" << endl;
                    } else {
                        tr.Clear_Tree(tr.GetRoot());
                        //tr.Root_Null();
                        tr.Load_Tree(&inFile);
                        cout << "OK" << endl;
                        inFile.close();
                    }
                } else {
                    cout << "ERROR: Unknown action" << endl;
                    
                }
            free(action);
            
        } else if(op == '?'){
            tr.Print_Tree(tr.GetRoot(), 0);
        }else {
            bool done = false;
            char k[260] = {'\0'};
            GetKey(k);
            char newkey[260] = {'\0'};
            newkey[0] = op;
            for(int i = 0; i < 260; i++){
                newkey[i+1] = k[i];
            }
            unsigned long long rez = tr.Find_k(newkey, &done);
            if (done) cout << "OK: " << rez << endl;
        }

        //tr.Print_Tree(tr.GetRoot(), 0);
       
        
    }//end while
    tr.Clear_Tree(tr.GetRoot());
}