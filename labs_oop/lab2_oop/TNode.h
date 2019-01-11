#ifndef TNODE_H
#define TNODE_H

#include "Octagon.h"

class TNode {
public:
    TNode(const Octagon& oct);
    TNode(const TNode& orig);
    friend std::ostream& operator<<(std::ostream& os, const TNode& obj);
    TNode *Push(Octagon oct, TNode **tree);
    void Print(TNode *tree, int n);
    Octagon GetOctagon() const;
    TNode *GetLeft();
    void SetLeft(TNode *left);
    TNode *GetRight();
    void SetRight(TNode *right);
    ~TNode();

private:
    Octagon octagon; 
    TNode *left;
    TNode *right;
};

#endif // TNODE_H