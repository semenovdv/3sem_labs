#ifndef NTREE_H
#define NTREE_H

#include <iostream>
#include <cstdlib>
#include <memory>
using namespace std;
 

template <class T> class NTNode{
public:
    NTNode();
	NTNode(T *value);
	template<class A> friend std::ostream& operator<<(std::ostream& os, const NTNode<A>& obj);
    
	void setson(NTNode<T> *son);
	void setbrother(NTNode<T> *brother);
	void setparent(NTNode<T> *parent);
	T *GetValue();

    virtual ~NTNode();

	bool empty();

    NTNode<T> *parent;
	NTNode<T> *brother;
	NTNode<T> *son;
    T* value;

	//static Tallocationblock ntnode_allocator;
};

template <class T> NTNode<T>::NTNode()
{
	this->parent = nullptr;
	this->brother = nullptr;
	this->son = nullptr;
	this->value = nullptr;
}
template <class T> NTNode<T>::NTNode(T *value)
{
	this->parent = nullptr;
	this->brother = nullptr;
	this->son = nullptr;
	this->value = value;
	std::cout << "NTNode created" << std::endl;
}

//template <class T> Tallocationblock NTNode<T>::ntnode_allocator(sizeof(NTNode<T>),100);

template <class T> void NTNode<T>::setson(NTNode<T> *son)
{
	this->son = son;
}
template <class T> void NTNode<T>::setbrother(NTNode<T> *brother)
{
	this->brother = brother;
}
template <class T>void NTNode<T>::setparent(NTNode<T> *parent)
{
	this->parent = parent;
}

template <class T> T* NTNode<T>::GetValue(){
	return this->value;
}

template <class T>  NTNode<T>::~NTNode()
{
	this->parent = nullptr;
	this->brother = nullptr;
	this->son = nullptr;
	this->value = nullptr;
}

template <class A> std::ostream& operator<<(std::ostream& os, const NTNode<A>& obj) {
    os << "[" << *obj.value << "]" << std::endl;
    return os;
}
/*
template <class T> void * NTNode<T>::operator new (size_t size) {
	return stackitem_allocator.allocate();
}


template <class T> void NTNode<T>::operator delete(void *p) {

	stackitem_allocator.deallocate(p);
}
*/

#include "Triangle.h"
template class NTNode<Triangle>;
template std::ostream& operator<<(std::ostream& os, const NTNode<Triangle>& obj);


#include "TIteratorNTree.h"

template <class A> class NTree {
public:
	
	NTree();

	void deleterec(NTNode<A> *node);
	void pushntree(A *value); // push
	A* deletelast(); //pop
	//A *Searchlast(); //pop
	
	bool empty();
	size_t size();

	int Sum();
    
	template <class T> friend std::ostream& operator<<(std::ostream& os,const NTree<T>& ntree);

	TIteratorNTree<NTNode<A>,A> begin() const;
	TIteratorNTree<NTNode<A>,A> end() const;
	
	NTNode<A> *root;

	virtual ~NTree();
};


template <class A> NTree<A>::NTree() : root(nullptr) {
	std::cout << "Ntree created" << std::endl;
}

template <class T> std::ostream& operator<<(std::ostream& os, const NTree<T>& ntree) {

    NTNode<T> *item = ntree.root; // TODO TODO TODO
    
    while(item!=nullptr)
    {
		
		os << *item;
		
		if((*item).brother != nullptr){
			os << *item->brother;
		}
		item = item->son;
    }
    
    return os;
}


template <class A> void NTree<A>::pushntree(A *value){
	NTNode<A> *nd(new NTNode<A>(value)); 
	if ((root == nullptr)) {
		root = nd;
		return;
	} else {
		NTNode<A> *Nodepath = root;
		if(root->son == nullptr){
			root->setson(nd);
			nd->parent = root;
			return;
		} else {
			Nodepath = Nodepath->son;
		}
		
        while (true){
                if (Nodepath->brother == nullptr) {
                    Nodepath->setbrother(nd);
                    return;
                } else {
                    if (Nodepath->son == nullptr) {
                        Nodepath->setson(nd);
						nd->parent = Nodepath;
                        return;
                    } else {
                        Nodepath = Nodepath->son;
                    }
                }
            
        }	
	}
}

template <class A> bool NTree<A>::empty(){
	return root == nullptr;
}

/*
template <class A> A* NTree<A>::Searchlast(){
	NTNode<A> *Nodepath = root;
	while(Nodepath->son != nullptr){
		Nodepath = Nodepath->son;
	}
	if(Nodepath->brother != nullptr){
		NTNode<A> *ret = Nodepath->brother;
		Nodepath->setbrother(nullptr);
		return ret->value;
	}
	if (Nodepath->parent != nullptr) Nodepath->parent->setson(nullptr);
	Nodepath->setparent(nullptr);

	return Nodepath->value;
}
*/
template <class A> A* NTree<A>::deletelast() {
	NTNode<A> *nodepath = root;
	while(nodepath->son != nullptr) {
		nodepath = nodepath->son;
	}
	if(nodepath->brother != nullptr){
		A *ret = nodepath->brother->value;
		nodepath->brother = nullptr;
		return ret;
	} else{
		A *ret = nodepath->value;
		if(nodepath->parent != nullptr){
			nodepath->parent->son = nullptr;
			nodepath->parent = nullptr;
		} else {
			root = nullptr;
		}
		return ret;
	}
}

template <class A> size_t NTree<A>::size() {
	size_t size = 0;
	
    NTNode<A> *tmp = root;
	while(tmp != nullptr){
		size++;
		if(tmp->brother != nullptr){
			size++;
		}
		tmp = tmp->son;
	}
	return size;

}

template <class A> int NTree<A>::Sum(){
	int result = 0;
	//TODO
    //for (auto a : * this) result += a->Square();
	if (root != nullptr){
		result += root->value->Square();
		NTNode<A> *tmp = root;
		tmp = tmp->son;
		while(tmp != nullptr){
			result += tmp->value->Square();
			if(tmp->brother != nullptr) result != tmp->brother->value->Square();
			tmp = tmp->son;
		}
	}

    return result;
}

template <class A> TIteratorNTree<NTNode<A>,A> NTree<A>::begin() const{
	return TIteratorNTree<NTNode<A>,A>(root);
}
template <class A> TIteratorNTree<NTNode<A>,A> NTree<A>::end() const{
	return TIteratorNTree<NTNode<A>,A>(nullptr);
}

template <class A> void NTree<A>::deleterec(NTNode<A> *node)
{
	if (node->brother != nullptr) {
		deleterec(node->brother);
	}
	if (node->son != nullptr) {
		deleterec(node->son);
	}
	node = nullptr;
}


template <class A> NTree<A>::~NTree()
{
	if (this->root) {
		deleterec(this->root);
	}
	this->root = nullptr;
}


#include "Triangle.h"
template class NTree<Triangle>;
template std::ostream& operator<<(std::ostream& os, const NTree<Triangle>& ntree); 

#endif