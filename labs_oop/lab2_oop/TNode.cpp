#include "TNode.h"
#include <iostream>



TNode::TNode(const Octagon& oct) {
    this->octagon = oct;
    this->left = nullptr;
    this->right = nullptr;
    std::cout << "Item creaed " << std::endl;
}

TNode::TNode(const TNode& orig) {
    this->octagon = orig.octagon;
    this->left = orig.left;
    this->right = orig.right;
    std::cout << "Copied " << std::endl;
}


Octagon TNode::GetOctagon() const {
    return this->octagon;
}

TNode *TNode::GetLeft() {
    return this->left;
}
void TNode::SetLeft(TNode *left) {
    this->left = left;
}

TNode *TNode::GetRight() {
    return this->right;
}

void TNode::SetRight(TNode *right) {
    this->right = right;
}
/*
void TNode::Print(TNode *tree, int n) {
    if(tree != nullptr) {
        TNode::Print(GetLeft(), n+1);
        for(int i =0; i < n; i++){
            std::cout << "  ";
        }
        std::cout << tree << std::endl;
        TNode::Print(GetRight(), n+1);
    }
}
*/

TNode::~TNode() {
    std::cout << "deleted" << std::endl;
    if(this->left != nullptr){
        delete this->left;
    }
    if(this->right != nullptr){
        delete this->right;
    }
}

std::ostream& operator<<(std::ostream& os, const TNode& obj) {
    if(obj.left != nullptr) os << *obj.left;
    os << "[" << obj.octagon << "]";
    if(obj.right != nullptr) os << *obj.right;
    return os;
}