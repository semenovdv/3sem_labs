#include <memory>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include "Figure.h"
#include <cstdlib>




template<class T> class TNode {
public:
    TNode(const std::shared_ptr<T>& item);
    //TNode(const TNode& orig);
    template<class A> friend std::ostream& operator<<(std::ostream& os, const TNode<T>& obj);
    
    //std::shared_ptr<TNode<T>> Push(std::shared_ptr<TNode<T>> &node, std::shared_ptr<TNode<T>> *tree);
    //void Print(TNode<T> *tree, int n);
    std::shared_ptr<T> GetItem() const;
    std::shared_ptr<TNode<T>> GetLeft();
    void SetLeft(std::shared_ptr<TNode<T>> left);
    std::shared_ptr<TNode<T>> GetRight();
    void SetRight(std::shared_ptr<TNode<T>> right);

    virtual ~TNode();

//private:
    std::shared_ptr<T> item;
    std::shared_ptr<TNode<T>> left;
    std::shared_ptr<TNode<T>> right;
};





template <class T> TNode<T>::TNode(const std::shared_ptr<T>& item) {
    this->item = item;
    this->left = nullptr;
    this->right = nullptr;
    std::cout << "Item creaed " << std::endl;
}

template <class T> std::shared_ptr<T> TNode<T>::GetItem() const {
    return this->item;
}

template <class T> std::shared_ptr<TNode<T>> TNode<T>::GetLeft() {
    return this->left;
}
template <class T> void TNode<T>::SetLeft(std::shared_ptr<TNode<T>> left) {
    this->left = left;
}

template <class T> std::shared_ptr<TNode<T>> TNode<T>::GetRight() {
    return this->right;
}

template <class T> void TNode<T>::SetRight(std::shared_ptr<TNode<T>> right) {
    this->right = right;
}


template <class T> TNode<T>::~TNode() {

    std::cout << "deleted" << std::endl;

}

template <class A> std::ostream& operator<<(std::ostream& os, const TNode<A>& obj) {
    if(obj.left != nullptr) std::cout << *obj.left;
    std::cout << "[" << *obj.item << "]" << std::endl;
    if(obj.right != nullptr) std::cout << *obj.right;
    return os;
}






template <class T> class TBTree {
public:
    TBTree();

    void Push(std::shared_ptr<T> &&item);
    bool empty();
    template <class A> friend std::ostream& operator<<(std::ostream& os, const TBTree<A>& tree);

    virtual ~TBTree();

private:
    std::shared_ptr<TNode<T>> root;
};



template <class T> TBTree<T>::TBTree(): root(nullptr) {
}

template <class T> std::ostream& operator<<(std::ostream& os, const TBTree<T>& tree){
    
    if(tree.root == nullptr){
        return os;
    }
    std::cout << *tree.root;

    return os;
}

template <class T> void TBTree<T>::Push(std::shared_ptr<T> &&fig) {
    std::shared_ptr<TNode<T>> tmp = this->root;
    std::shared_ptr<TNode<T>> parent = nullptr;
    while(tmp != nullptr) {
        parent = tmp;
        if (fig->Square() < tmp->GetItem()->Square()){
            tmp = tmp->GetLeft();
        } else if(fig->Square() > tmp->GetItem()->Square()) {
            tmp = tmp->GetRight();
        } else {
            std::cout << "exists" << std::endl;
            return;
        }
    }

    tmp = std::make_shared<TNode<T>>(fig);
    if(this->root == nullptr) {
        this->root = tmp;
    } else if(parent->GetItem()->Square() > fig->Square()){
        parent->SetLeft(tmp);
    } else {
        parent->SetRight(tmp);
    }
    return;
}

template <class T> bool TBTree<T>::empty() {
    return root == nullptr;
}


template <class T> TBTree<T>::~TBTree() {
}

