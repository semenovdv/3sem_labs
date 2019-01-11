#ifndef RBTREE_H
#define RBTREE_H

//#include "TQueue.h"
#include "Node.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <algorithm>
#include <cstring>

class TRBTree {
public:
    TRBTree(){
        root = nil; 
    };
    void Insert_kv(char* key, unsigned long long value);
    void Delete_k(char* key);
    unsigned long long Find_k(char* key, bool *done);
    void Print_Tree(Node *root, long long n);
    void Save_Tree(std::ofstream *outfile, Node *node);
    void Load_Tree(std::ifstream *infile);
    void Clear_Tree(Node *node);

    Node *GetRoot() const{
        return (this->root);
    }
    
    ~TRBTree(){
            
    };

protected:
    // повороты
    void RotateLeft(Node *); // done
    void RotateRight(Node *); // done
    // методы вставки
    Node *InsertHidden(Node *, Node *);
    void FixInsertRBTree(Node *);
    // мтоды удаления
    Node *Find(char* key, bool *found);
    void DeleteHidden(Node *node);
    void FixDeleteRBTree(Node *, Node *, bool);
    void Load_Tree_helper(std::ifstream *infile, Node *node);
    
private:
    Node *root;
    char nilstr[4] = "nil";
    Node undernil = {BLACK, &undernil, &undernil, &undernil, nilstr, 0};
    Node *nil = &undernil;
    
};

#endif // RBTREE_H
