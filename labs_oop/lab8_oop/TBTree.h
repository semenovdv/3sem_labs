#include <memory>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <thread>
#include <mutex>

#include <cstdlib>




template<class T> class TNode {
public:
    TNode(T* item);
    
    template<class A> friend std::ostream& operator<<(std::ostream& os, const TNode<T>& obj);
    
    
    T* GetItem() const;

    
    virtual ~TNode();
    

//private:
    T* item;
    TNode<T>* left;
    TNode<T>* right;
    TNode<T>* parent;
};

template <class T> TNode<T>::TNode(T* item) {
    this->item = item;
    this->left = nullptr;
    this->right = nullptr;
    this->parent = nullptr;
    //std::cout << "Item creaed " << std::endl;
}

template <class T> T* TNode<T>::GetItem() const {
    return this->item;
}

template <class T> TNode<T>::~TNode() {

    std::cout << "deleted" << std::endl;

}

template <class A> std::ostream& operator<<(std::ostream& os, const TNode<A>& obj) {
    std::cout << "[" << *obj.item << "]";
    return os;
}


template <class T> class TBTree {
public:
    TBTree();

    void Push(T* item);
    bool empty();
   
    template <class A> friend std::ostream& operator<<(std::ostream& os, const TBTree<A>& tree);

    void dfs_parallel(TNode<T> *node);
    void dfs(TNode<T> *node);
    template <class A> void Print_Tree(const TNode<A>& obj);
    void thr_func(TNode<T> *node);

    virtual ~TBTree();

    TNode<T>* root;
};



template <class T> TBTree<T>::TBTree(): root(nullptr) {
}

template <class T> std::ostream& operator<<(std::ostream& os, const TBTree<T>& tree){
    
    if(tree.root == nullptr){
        return os;
    }
    Print_Tree(*tree.root);

    return os;
}

template <class A> void Print_Tree(const TNode<A>& obj) { 
    if(obj.left!= nullptr) Print_Tree(*obj.left);
    std::cout << *obj << std::endl;
    if(obj.right!= nullptr) Print_Tree(*obj.right);
    return;
}


template <class T> void TBTree<T>::Push(T* value) {
    TNode<T>* tmp = this->root;
    TNode<T>* paren = nullptr;
    TNode<T> *insertnode(new TNode<T>(value));
    while(tmp != nullptr) {
        if (value->Square() < tmp->GetItem()->Square()){
            paren = tmp;
            tmp = tmp->left;
        } else if(value->Square() >= tmp->GetItem()->Square()) {
            paren = tmp;
            tmp = tmp->right;
        } else {
            std::cout << "error" << std::endl;
        }
    }

    
    if(this->root == nullptr) {
        this->root = insertnode;
    } else if(paren->GetItem()->Square() > value->Square()){
        paren->left = insertnode;
        insertnode->parent = paren;
    } else {
        paren->right = insertnode;
        insertnode->parent = paren;
    }
    return;
}

template <class T> bool TBTree<T>::empty() {
    return root == nullptr;
}


template <class T> TBTree<T>::~TBTree() {
}

template <class T> void TBTree<T>::thr_func(TNode<T> *node){
    dfs_parallel(node);
    return;
}

template <class T> void TBTree<T>::dfs_parallel(TNode<T> *node) {
    if(node != nullptr){
        std::thread th;
        std::cout << *node->item << std::endl;
        if (node->right != nullptr){
            dfs_parallel(node->right);     
        }
        if (node->left != nullptr){
            th = std::thread(&TBTree<T>::thr_func, this, node->left);
            th.join();
        }
        
    }
    return;
}

template <class T> void TBTree<T>::dfs(TNode<T> *node) {

    if (node->left != nullptr){
        dfs(node->left);
    }
    std::cout << *node->item << std::endl;
    if (node->right != nullptr){
        dfs(node->right);
    }
}


