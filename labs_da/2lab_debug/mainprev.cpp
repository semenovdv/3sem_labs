#include "TRBTree.h"
#include <string.h>
#include <iostream>
#include <fstream>


using std::cout;
using std::cin;
using std::string;
using std::endl;

typedef struct {
	bool is_list;
    nodeColor color; 
    unsigned long long m_value;
    char m_key[257];
} PrMin;

int main() {
    std::ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    char save[5] = "Save";
    char load[5] = "Load";
    //char print[6] = "Print";

    char *op = (char*)malloc(sizeof(char) * 300);
    char *ptr = 0;

    TRBTree tr;

    while (true){
        for(int i = 0; i < 300; i++){
            op[i] = '\0';
        }
        cin.getline(op, 300);
        
        if (!cin.good()) break;
        ptr = strtok(op, " ");
        
        
        if (strcmp(ptr,"+") == 0) {
            ptr = strtok(nullptr, " ");
            if( ptr == nullptr){
                bool done = false;
                unsigned long long rez = tr.Find_k(op, &done);
                if (done) cout << "OK: " << rez << endl;

            } else {
                unsigned long long v = 0;;
                char k[257];

                for(int i = 0; i < 257; i++){
                    k[i] = '\0';
                }
                strcpy(k, ptr);
                ptr = strtok(nullptr , " ");
                v = std::stoull(ptr);

                try{
                    tr.Insert_kv(k, v);

                } catch(std::bad_exception&) {
                    cout << "ERROR: Not enough memory" << endl;
                    tr.Clear_Tree(tr.GetRoot());
                    return 0;
                }
            }

        } else if (strcmp(ptr, "-") == 0) {
            ptr = strtok(nullptr, " ");
            if( ptr == nullptr){
                bool done = false;
                unsigned long long rez = tr.Find_k(op, &done);
                if (done) cout << "OK: " << rez << endl;

            } else {
                char k[257];
                for(int i = 0; i < 257; i++){
                    k[i] = '\0';
                }
                strcpy(k, ptr);
                tr.Delete_k(k);
            }

        } else if (strcmp(ptr, "!") == 0){
            ptr = strtok(nullptr, " ");
            if( ptr == nullptr){
                bool done = false;
                unsigned long long rez = tr.Find_k(op, &done);
                if (done) cout << "OK: " << rez << endl;

            } else {
                char *action = (char*)malloc(sizeof(char) * 257);
                strcpy(action, ptr);
                ptr = strtok(nullptr, " ");

                if (strcmp(action, save) == 0) {
                    char path[257] = {'\0'};
                    strcpy(path, ptr);
                    std::ofstream outFile(path, std::ios::binary | std::ios::out);

                    if (! outFile.is_open()) {
                        cout << "ERROR: Couldn`t create file." << endl;
                    } else {
                        tr.Save_Tree(&outFile, tr.GetRoot());
                        outFile.close();
                        cout << "OK" << endl;
                    }

                } else if (strcmp(action, load) == 0) {
                    char path[257] = {'\0'};
                    strcpy(path, ptr);
                    std::ifstream inFile(path, std::ios::binary | std::ios::in);

                    if (! inFile.is_open()) {
                        cout << "ERROR: Unable to open file." << endl;
                    } else {
                        tr.Clear_Tree(tr.GetRoot());
                        //tr.Root_Null();
                        tr.Load_Tree(&inFile);
                        inFile.close();
                    }
                } else {
                    cout << "ERROR: Unknown action." << endl;
                    
                }
            }
            
        }/*else if(strcmp(print, op) == 0){
            tr.Print_Tree(tr.GetRoot(), 0);
        } */
        else {
            bool done = false;
            unsigned long long rez = tr.Find_k(op, &done);
            if (done) cout << "OK: " << rez << endl;
        }

        //tr.Print_Tree(tr.GetRoot(), 0);
       
        
    }//end while
    tr.Clear_Tree(tr.GetRoot());
}