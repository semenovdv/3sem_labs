#ifndef NODE_H
#define NODE_H
#include <string.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

typedef enum { BLACK, RED } nodeColor;

class Node {
    public:
        nodeColor color; // enumed color
        Node *left;
        Node *right;
        Node *parent;
        unsigned long long m_value = 0;
        char *m_key = nullptr;

        Node(char *key, unsigned long long value){
            int i = 0;
            if(key != nullptr){
                int len_key = 0;
                len_key = strlen(key);
                //cout << len_key << endl;
                m_key = (char*)malloc((len_key+1) * sizeof(char));
                for(i = 0; i < len_key; i++){
                    m_key[i] = key[i];
                }
            }
            m_key[i] = '\0'; // valgrind lenkey+1        
            m_value = value;
            color = RED;
        };
        
        Node(nodeColor color, Node *left, Node *right, 
            Node *parent,  char *key, unsigned long long value){
            int i = 0;
            if(key != nullptr){
                int len_key = 0;
                len_key = strlen(key);
                //cout << len_key << endl;
                m_key = (char*)malloc((len_key+1) * sizeof(char));
                for(i = 0; i < len_key; i++){
                    m_key[i] = key[i];
                }
            }
            m_key[i] = '\0'; // valgrind lenkey+1        
            m_value = value;
            this->color = color;
            this->left = left;
            this->right = right;
            this->parent = parent;
        };
        
        Node(){ 
        };
        ~Node(){
            free(m_key); // valgrind  // double free or corruption ???
            
        };
    };
#endif

