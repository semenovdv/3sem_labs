#include "TBTree.h"

TBTree::TBTree(): root(nullptr) {
}

std::ostream& operator<<(std::ostream& os, const TBTree& tree){
    
    if(tree.root == nullptr){
        return os;
    }
    os << *tree.root;

    return os;
}

void TBTree::Push(std::shared_ptr<Figure> &&fig) {
    std::shared_ptr<TNode> tmp = this->root;
    std::shared_ptr<TNode> parent = nullptr;
    while(tmp != nullptr) {
        parent = tmp;
        if (fig->Square() < tmp->GetFigure()->Square()){
            tmp = tmp->GetLeft();
        } else if(fig->Square() > tmp->GetFigure()->Square()) {
            tmp = tmp->GetRight();
        } else {
            std::cout << "exists" << std::endl;
            return;
        }
    }

    tmp = std::make_shared<TNode>(fig);
    if(this->root == nullptr) {
        this->root = tmp;
    } else if(parent->GetFigure()->Square() > fig->Square()){
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
}

