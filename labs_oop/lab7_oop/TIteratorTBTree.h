#ifndef TITERATORTBTREE_H
#define TITERATORTBTREE_H

#include <memory>
#include <iostream>
#include "Stack.h"

template<class node, class T, class TT>
class TIterator {
public:
    TIterator(node *n) {
        node_ptr = n;
    }

    T* operator * (){
        return node_ptr->GetValue();
    }

    T* operator -> (){
        return node_ptr->GetValue();
    }

    void operator ++ (){
        if (node_ptr->GetLeft() != nullptr){
            if(node_ptr->GetRight() != nullptr) stack.push(node_ptr->GetRight());
            node_ptr = node_ptr->GetLeft();
            return; //TIterator(node_ptr);
        } else if (node_ptr->GetRight() != nullptr){
            if(node_ptr->GetLeft() != nullptr) stack.push(node_ptr->GetLeft());
            node_ptr = node_ptr->GetRight();
            return; //TIterator(node_ptr);
        } else {
            if(!stack.empty()){
                node_ptr = stack.pop();
                return; // TIterator(node_ptr);
            } else {
                node_ptr = nullptr;
                return; // TIterator(nullptr);
            }
        }
        
        
        //node_ptr = node_ptr->GetNext();
    }

    TIterator operator ++ (int){
        TIterator iter(*this);
        ++(*this);
        return iter;
    }

    bool operator == (TIterator const& i){
        return node_ptr == i.node_ptr;
    }

    bool operator != (TIterator const& i){
        return !(*this == i);
    }

private:
    node *node_ptr;
    TStack<node> stack;    

};

#endif