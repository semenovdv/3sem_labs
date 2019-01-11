#ifndef NTREE_H
#define NTREE_H

#include <iostream>
#include <cstdlib>
#include <memory>

using namespace std;
 

template <class T> class NTNode{
public:
    NTNode();
	NTNode(T value);
	void SetSon(NTNode<T> *son);
	void SetBrother(NTNode<T> *brother);
	void SetParent(NTNode<T> *parent);
	NTNode<T> *Parent();
	NTNode<T> *Son();
	NTNode<T> *Brother();
    virtual ~NTNode();

    NTNode<T> *parent;
	NTNode<T> *brother;
	NTNode<T> *son;
    T value;
};

template <class T> NTNode<T>::NTNode()
{
	this->parent = nullptr;
	this->brother = nullptr;
	this->son = nullptr;
	this->value = nullptr;
}
template <class T> NTNode<T>::NTNode(T value)
{
	this->parent = nullptr;
	this->brother = nullptr;
	this->son = nullptr;
	this->value = value;
}
template <class T> void NTNode<T>::SetSon(NTNode<T> *son)
{
	this->son = son;
}
template <class T>void NTNode<T>::SetBrother(NTNode<T> *brother)
{
	this->brother = brother;
}
template <class T>void NTNode<T>::SetParent(NTNode<T> *parent)
{
	this->parent = parent;
}


template <class T>  NTNode<T>::~NTNode()
{
	this->parent = nullptr;
	this->brother = nullptr;
	this->son = nullptr;
	this->value = nullptr;
}


template <class A> class NTree {
	
public:
	NTNode<A> *root;
	int size;
	NTree();
	void deleterec(NTNode<A> *node);
	void Insert(A value);
	NTNode<A> *Search(NTNode<A> *pointer);
	NTNode<A> *Searchlast();
	void Deletelast();
	int Size();
	
	
	virtual ~NTree();
};

template <class A> int NTree<A>::Size(){
	std::cout << this->size << endl;
}

template <class A> NTree<A>::NTree()
{
	this->size = 0;
	this->root = nullptr;
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

template <class A> NTNode<A>* NTree<A>::Searchlast(){
	NTNode<A> *Nodepath = root;
	while(Nodepath->son != nullptr){
		Nodepath = Nodepath->son;
	}
	if(Nodepath->brother != nullptr){
		NTNode<A> *ret = Nodepath->brother;
		Nodepath->SetBrother(nullptr);
		size--;
		Size();
		return ret;
	}
	if (Nodepath->parent != nullptr) Nodepath->parent->SetSon(nullptr);
	Nodepath->SetParent(nullptr);

	size--;
	Size();
	return Nodepath;
}

template <class A> void NTree<A>::Insert(A value){
	size++;
	Size();
	NTNode<A> *nd(new NTNode<A>(value)); 
	if ((root == nullptr)) {
		root = nd;
		return;
	}
	else {
		NTNode<A> *Nodepath = root;
		if(root->son == nullptr){
			root->SetSon(nd);
			return;
		} else {
			Nodepath = Nodepath->son;
		}
		
        while (true){
                if (Nodepath->brother == nullptr) {
                    Nodepath->SetBrother(nd);
                    break;
                } else {
                    if (Nodepath->son == nullptr) {
                        Nodepath->SetSon(nd);
                        break;
                    } else {
                        Nodepath = Nodepath->son;
                    }
                }
            
        }	
	}
}

template <class A> void NTree<A>::Deletelast()
{
	NTNode<A> *deleting = this->Searchlast();
	size--;

	deleterec(deleting);
}

template <class A> NTree<A>::~NTree()
{
	if (this->root) {
		deleterec(this->root);
	}
	size = 0;
	this->root = nullptr;
}



#endif