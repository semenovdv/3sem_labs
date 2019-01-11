#include "TBTree.h"

TBTree::TBTree(): root(nullptr) {
}

TBTree::TBTree(const TBTree& orig) {
    root = orig.root;
}

std::ostream& operator<<(std::ostream& os, const TBTree& tree){
    
    if(tree.root == nullptr){
        return os;
    }
    os << *tree.root;

    return os;
}

void TBTree::Push(Octagon &oct) {
    TNode *tmp = this->root;
    TNode *parent = nullptr;
    while(tmp != nullptr) {
        parent = tmp;
        if (oct.Square() < tmp->GetOctagon().Square()){
            tmp = tmp->GetLeft();
        } else if(oct.Square() > tmp->GetOctagon().Square()) {
            tmp = tmp->GetRight();
        } else {
            std::cout << "exists" << std::endl;
            return;
        }
    }

    tmp = new TNode(oct);
    if(this->root == nullptr) {
        this->root = tmp;
    } else if(parent->GetOctagon().Square() >oct.Square()){
        parent->SetLeft(tmp);
    } else {
        parent->SetRight(tmp);
    }
    return;
}

bool TBTree::empty() {
     return root == nullptr;
}

TBTree::~TBTree() {
    delete this->root;
}

