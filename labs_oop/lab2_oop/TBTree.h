#ifndef TBTREE_H
#define TBTREE_H 

#include "TNode.h"

class TBTree {
public:
    TBTree();
    TBTree(const TBTree& orig);

    void Push(Octagon &octagon);
    bool empty();
    friend std::ostream& operator<<(std::ostream& os, const TBTree& tree);

    virtual ~TBTree();

private:
    TNode *root;
};

/*
void TNode::Print(TNode *tree, int n) {
    if(tree != nullptr) {
        TNode::Print(GetLeft(tree), n+1);
        for(int i =0; i < n; i++){
            std::cout << "  ";
        }
        std::cout << tree << std::endl;
        TNode::Print(GetRight(tree), n+1);
    }
}*/

#endif //TTREE_H