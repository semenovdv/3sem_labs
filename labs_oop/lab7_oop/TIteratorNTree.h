#ifndef TITERATORNTREE_H
#define TITERATORNTREE_H

#include <memory>
#include <iostream>

template<class node, class T>
class TIteratorNTree {
public:
    TIteratorNTree(node *n) {
        node_ptr = n;
    }

    T* operator * (){
        return node_ptr->GetValue();
    }

    T* operator -> (){
        return node_ptr->GetValue();
    }

    void operator ++ (){
        if (node_ptr->brother != nullptr){
            curr_ptr = node_ptr;
            node_ptr = node_ptr->brother;
        } else if(node_ptr->son != nullptr){
            node_ptr = node_ptr->son;
        } else if(node_ptr->parent == nullptr) {
            if (curr_ptr->son != nullptr) {
                node_ptr = curr_ptr->son;
            } else {
                return; //TIteratorNTree(nullptr);
            }
        } else {
            return; // TIteratorNTree(nullptr);
        }
        
        
    }

    TIteratorNTree operator ++ (int){
        TIteratorNTree iter(*this);
        ++(*this);
        return iter;
    }

    bool operator == (TIteratorNTree const& i){
        return node_ptr == i.node_ptr;
    }

    bool operator != (TIteratorNTree const& i){
        return !(*this == i);
    }

private:
    node *node_ptr;
    node *curr_ptr = nullptr;    

};

#endif