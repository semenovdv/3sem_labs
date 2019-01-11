#include <map>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <memory>
#include <ctime> 

using namespace std;

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
/*
typedef struct {
	bool is_list;
    nodeColor color; 
    unsigned long long m_value;
    char m_key[260];
} PrMin;
*/

int main() {
    clock_t start_my = clock();
    std::ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    char save[5] = "save";
    char load[5] = "load";

    

    std::map<string, unsigned long long> mymap;

    while (true){

        string op = "\n";

        while(op == "\n" || op == " "){
            if(!(cin >> op)){
                clock_t end_my = clock();
                std::cout <<"map " << (double)(-(start_my - end_my)) / CLOCKS_PER_SEC << std::endl;
                return 0;
            }
        }
       
       if(cin.eof()){
           return 0;
       }
        
        if (op == "+") {
            
            
            unsigned long long v = 0;;
            string k;
                //GetKey(k);
                cin >> k;
                locale loc;
                for (std::string::size_type i=0; i<k.length(); ++i)
                    k[i] = std::tolower(k[i],loc);
                    
            cin >> v;
            mymap.insert(pair<string, unsigned long long>(k ,v));
            cout << "OK" << endl;
            
            
        } else if (op == "-") {
            
        
                string k;
                //GetKey(k);
                cin >> k;
                locale loc;
                for (std::string::size_type i=0; i<k.length(); ++i)
                    k[i] = std::tolower(k[i],loc);

                if (mymap.erase(k)){ cout << "Ok" << endl;}
                else cout << "NoSuchWord" << endl;

                
            
            
        } /*else if (op == '!'){
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
                        tr.Load_Tree(&inFile);
                        cout << "OK" << endl;
                        inFile.close();
                    }
                } else {
                    cout << "ERROR: Unknown action" << endl;
                    
                }
            free(action);
            */
        /*} else if(op == '?'){
            tr.Print_Tree(tr.GetRoot(), 0);
        }*/
       else {
           locale loc;
            for (std::string::size_type i=0; i<op.length(); ++i)
                    op[i] = std::tolower(op[i],loc);
            
            if(mymap.find(op) != mymap.end()){
                cout << "Ok: ";
                cout << mymap.find(op)->second << endl;
            } else {
                cout << "NoSuchWord" << endl;
            }
            
            
        }

        //tr.Print_Tree(tr.GetRoot(), 0);
       
        
    }//end while
    //tr.Clear_Tree(tr.GetRoot());
    
}