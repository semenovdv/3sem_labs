#include <memory>
#include <cstdlib>
#include <iostream>
#include <math.h>

#include "TIteratorTBTree.h"
#include "IRemoveCriteria.h"


template<class T> class TNode {
public:
    TNode(T* item);    
    
    T* GetValue();
    TNode<T> *GetLeft();
    TNode<T> *GetRight();

    void SetLeft(TNode<T> *left); // PushBack
    void SetRight(TNode<T> *right);


    template<class A> friend std::ostream& operator<<(std::ostream& os, const TNode<T>& obj);

   //Push Back must be in TBTree
   
   // void PushBack(TNode<T> *next);
/*
    void *operator new(size_t size);
    void operator delete(void *p);
*/
    virtual ~TNode();

//private:
    T* item;
    TNode<T> *left;
    TNode<T> *right;
    TNode<T> *parent;
};



template <class T> TNode<T>::TNode( T* item) {
    this->item = item;
    this->left = nullptr;
    this->right = nullptr;
    this->parent = nullptr;
    std::cout << "Item creaed " << std::endl;
}

template <class T> T* TNode<T>::GetValue() {
    return item;
}

template <class T> TNode<T>* TNode<T>::GetLeft() {
    return this->left;
}
template <class T> void TNode<T>::SetLeft(TNode<T> *left) {
    this->left = left;
}

template <class T> TNode<T>* TNode<T>::GetRight() {
    return this->right;
}

template <class T> void TNode<T>::SetRight(TNode<T> *right) {
    this->right = right;
}
/*
template <class T> void TNode<T>::PushBack(std::shared_ptr<TListItem> next) {
    if (_next != nullptr) {
        _next->PushBack(next);
    } else {
        _next = next;
    }
}*/

template <class T> TNode<T>::~TNode() {

    std::cout << "deleted" << std::endl;

}


template <class A> std::ostream& operator<<(std::ostream& os, const TNode<A>& obj) {
    if(obj.left != nullptr) std::cout << *obj.left;
    std::cout << "[" << *obj.item << "]" << std::endl;
    if(obj.right != nullptr) std::cout << *obj.right;
    return os;
}

/*
template <class T> void * TNode<T>::operator new (size_t size) {
	std::cout << "Allocated :" << size << "bytes" << std::endl;
	return malloc(size);
}

template <class T> void TNode<T>::operator delete(void *p) {
	std::cout << "Deleted" << std::endl;
	free(p);
}*/


#include "NTree.h"
#include "Triangle.h"
template class TNode<NTree<Triangle>>;
template class TNode<Triangle>;


template <class T, class TT> class TBTree {
public:
    TBTree();

    void InsertSubitem(TT* value);
    void RemoveSubitem(IRemoveCriteria<TT> * criteria);
    void PushBack(T *value); // PushBack
    bool Remove(T* value);
    size_t Size();

    
    template <class A, class AA> friend std::ostream& operator<<(std::ostream& os, TBTree<A, AA>& tree);

    TIterator<TNode<T>,T, TT> begin() const;
    TIterator<TNode<T>,T, TT> end() const;

    virtual ~TBTree();

private:
    TNode<T> *root;
};



template <class T, class TT> TBTree<T, TT>::TBTree(): root(nullptr) {
}

template <class T, class TT> void TBTree<T, TT>::RemoveSubitem(IRemoveCriteria<TT> * criteria) {
    std::cout << "---------------------->" << std::endl;
    for (auto i : * this) {
        T copy;
        while (!i->empty()) {
            TT *value = i->deletelast();

            if (criteria->isIt(&*value))
                std::cout << "TBtree: Delete element " << *value << std::endl;
            else {
                copy.pushntree((new TT(*value)));
            }
        }

        while (!copy.empty())
            i->pushntree(new TT(*copy.deletelast()));

    }
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
}

template <class T, class TT> void TBTree<T, TT>::InsertSubitem(TT* value) {

    bool inserted = false;

    if (root != nullptr) {

        for (auto i : * this) {
            //std::cout << i->size() << std::endl;
            if (i->size() < 5) {
                std::cout << "i size " << i->size() << std::endl;
                i->pushntree(value);
                std::cout << *value << std::endl;
                std::cout << "TBtree: Add Element in ntree:" << i->size() << std::endl;
                inserted = true;
                break; //TODO
            } else {
                if( i == nullptr){
                    break;
                }
            }
            //std::cout << "is TRUE? " << inserted << std::endl;

        }
        std::cout << "is true? " << inserted << std::endl;
    }

    if (!inserted) {
        std::cout << "TBtree: New tbtree element created" << std::endl;
        T* t_value = new T();
        t_value->pushntree(value);
        PushBack(t_value);
    }
}
/*

*/

template <class T, class TT> void TBTree<T, TT>::PushBack(T *value) {
    TNode<T> *tmp = this->root;
    TNode<T> *paren = nullptr;
    TNode<T> *insertnode(new TNode<T>(value));
    while(tmp != nullptr) {
        if (value->Sum() < tmp->GetValue()->Sum()){
            paren = tmp;
            tmp = tmp->GetLeft();
        } else if(value->Sum() >= tmp->GetValue()->Sum()) {
            paren = tmp;
            tmp = tmp->GetRight();
        } else {
            std::cout << "ERROR INCLUDING NTREE TO TBTREE" << std::endl;
        }
    }

    if(this->root == nullptr) {
        this->root = insertnode;
    } else if(paren->GetValue()->Sum() > value->Sum()){
        paren->SetLeft(insertnode);
        insertnode->parent = paren;
    } else {
        paren->SetRight(insertnode);
        insertnode->parent = paren;
    }
    return;
}

template <class T, class TT> bool TBTree<T, TT>::Remove(T* value){
    TNode<T> *tmp = root;
    TNode<T> *parent = nullptr;
    bool result = false;

    while((tmp != nullptr) && (!result)) {
        parent = tmp;
        if (value->Sum() < tmp->GetValue()->Sum()){
            tmp = tmp->GetLeft();
        } else if(value->Sum() > tmp->GetValue()->Sum()) {
            tmp = tmp->GetRight();
        } else {
            if(tmp != nullptr){
                if(tmp->GetLeft() == nullptr) {
                    tmp->GetRight()->parent = tmp->parent;
                    if(tmp->parent->GetLeft() == tmp){
                        tmp->parent->SetLeft(tmp->GetRight());
                    } else {
                        tmp->parent->SetRight(tmp->GetRight());
                    }
                    
                } else if(tmp->GetRight() == nullptr) {
                    tmp->GetLeft()->parent = tmp->parent;
                    if(tmp->parent->GetLeft() == tmp){
                        tmp->parent->SetLeft(tmp->GetLeft());
                    } else {
                        tmp->parent->SetRight(tmp->GetLeft());
                    }
                } else {
                    TNode<T> *find = tmp;
                    find = find->GetRight();
                    while(find->GetLeft() != nullptr){
                        find = find->GetLeft();
                    }
                    if(find->parent->GetLeft() == find){
                        find->parent->SetLeft(find->GetRight());
                        if(find->GetRight() != nullptr) find->GetRight()->parent = find->parent;
                    }else {
                        find->parent->SetRight(find->GetRight());
                        if(find->GetRight() != nullptr) find->GetRight()->parent = find->parent;
                    }

                    find->parent = tmp->parent;
                    tmp->parent = nullptr;
                    find->SetLeft(tmp->GetLeft());
                    tmp->SetLeft(nullptr);
                    find->SetRight(tmp->GetRight());
                    tmp->SetRight(nullptr);

                }
            } else {
                std::cout << "dont found" << endl;
            }
            //delete(tmp);
        }
    }
}

template <class T, class TT> size_t TBTree<T, TT>::Size() {
    size_t result = 0;

    for (auto a : * this) result++;

    return result;
}

template<class T, class TT> TIterator<TNode<T>, T, TT> TBTree<T, TT>::begin() const {
    return TIterator<TNode<T>, T, TT>(root);
}

template <class T, class TT> TIterator<TNode<T>,T, TT> TBTree<T, TT>::end() const{
    return TIterator<TNode<T>, T, TT>(nullptr);
}

template <class T, class TT> TBTree<T, TT>::~TBTree() {
}

template <class T, class TT> std::ostream& operator<<(std::ostream& os, TBTree<T, TT>& tree){

    for(auto i :tree) std::cout << *i << std::endl;
    return os;
}


#include "NTree.h"
#include "Triangle.h"

template class TBTree<NTree<Triangle>, Triangle>;
template std::ostream& operator<<(std::ostream &os, TBTree<NTree<Triangle>,Triangle> &tree);