#ifndef TITERATOR_H
#define TITERATOR_H

#include <memory>
#include <iostream>
#include "Stack.h"

template<class node, class T>
class TIterator {
public:
    TIterator(std::shared_ptr<node> n) {
        node_ptr = n;
    }

    std::shared_ptr<T> operator * (){
        return node_ptr->GetItem();
    }

    std::shared_ptr<T> operator -> (){
        return node_ptr->GetItem();
    }

    TIterator operator ++ (int){
        TIterator iter(*this);
        ++(*this);
        return iter;
    }

    void operator ++ (){
        if (node_ptr->GetLeft() != nullptr){
            if(node_ptr->GetRight() != nullptr) stack.push(node_ptr->GetRight());
            node_ptr = node_ptr->GetLeft();

        } else if (node_ptr->GetRight() != nullptr){
            node_ptr = node_ptr->GetRight();
        } else {
            if(!stack.empty()){
                node_ptr = stack.pop();
            } else {
                node_ptr = nullptr;
            }
        }
    }

   
    bool operator == (TIterator const& i){
        return node_ptr == i.node_ptr;
    }

    bool operator != (TIterator const& i){
        return !(*this == i);
    }

private:
    std::shared_ptr<node> node_ptr;
    TStack<node> stack;    

};

#endif