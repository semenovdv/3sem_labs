#include "TNode.h"
#include <iostream>



TNode::TNode(const std::shared_ptr<Figure>& fig) {
    this->figure = fig;
    this->left = nullptr;
    this->right = nullptr;
    std::cout << "Item creaed " << std::endl;
}


std::shared_ptr<Figure> TNode::GetFigure() {
    return this->figure;
}

std::shared_ptr<TNode> TNode::GetLeft() {
    return this->left;
}
void TNode::SetLeft(std::shared_ptr<TNode> left) {
    this->left = left;
}

std::shared_ptr<TNode> TNode::GetRight() {
    return this->right;
}

void TNode::SetRight(std::shared_ptr<TNode> right) {
    this->right = right;
}


TNode::~TNode() {
    
    std::cout << "deleted" << std::endl;

}

std::ostream& operator<<(std::ostream& os, const TNode& obj) {
    if(obj.left != nullptr) std::cout << *obj.left;
    std::cout << "[" << *obj.figure << "]" << std::endl;
    if(obj.right != nullptr) std::cout << *obj.right;
    return os;
}