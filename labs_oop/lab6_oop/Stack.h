#ifndef TSTACK_H
#define	TSTACK_H

//#include "TIterator.h"

#include <memory>
#include <iostream>


template<class T> class TStackItem {
public:
    TStackItem(const std::shared_ptr<T>& triangle);
    template<class A> friend std::ostream& operator<<(std::ostream& os, const TStackItem<A>& obj);
    
    std::shared_ptr<TStackItem<T>> SetNext(std::shared_ptr<TStackItem> &next);
    std::shared_ptr<TStackItem<T>> GetNext();
    std::shared_ptr<T> GetValue() const;
    void * operator new (size_t size);
    void operator delete(void *p);
    
    virtual ~TStackItem();
private:
    std::shared_ptr<T> item;
    std::shared_ptr<TStackItem<T>> next;
};




template <class T> TStackItem<T>::TStackItem(const std::shared_ptr<T>& item) {
    this->item = item;
    this->next = nullptr;
    std::cout << "Stack item: created" << std::endl;
}

template <class T> std::shared_ptr<TStackItem<T>> TStackItem<T>::SetNext(std::shared_ptr<TStackItem<T>> &next) {
    std::shared_ptr<TStackItem < T>> old = this->next;
    this->next = next;
    return old;
}

template <class T> std::shared_ptr<T> TStackItem<T>::GetValue() const {
    return this->item;
}

template <class T> std::shared_ptr<TStackItem<T>> TStackItem<T>::GetNext() {
    return this->next;
}

template <class T> TStackItem<T>::~TStackItem() {
    std::cout << "Stack item: deleted" << std::endl;

}

template <class A> std::ostream& operator<<(std::ostream& os, const TStackItem<A>& obj) {
    os << "[" << *obj.item << "]" << std::endl;
    return os;
}

template <class T> void * TStackItem<T>::operator new (size_t size) {
	std::cout << "Allocated :" << size << "bytes" << std::endl;
	return malloc(size);
}

template <class T> void TStackItem<T>::operator delete(void *p) {
	std::cout << "Deleted" << std::endl;
	free(p);
}


template <class T> class TStack {
public:
    TStack();
    
    void push(std::shared_ptr<T> &&item);
    bool empty();
    std::shared_ptr<T> pop();
    
    
    
    
    template <class A> friend std::ostream& operator<<(std::ostream& os,const TStack<A>& stack);
    virtual ~TStack();
private:
    
    std::shared_ptr<TStackItem<T>> head;
};

template <class T> TStack<T>::TStack() : head(nullptr) {
}


template <class T> std::ostream& operator<<(std::ostream& os, const TStack<T>& stack) {

    std::shared_ptr<TStackItem<T>> item = stack.head;
    
    while(item!=nullptr)
    {
      os << *item;
      item = item->GetNext();
    }
    
    return os;
}

template <class T> void TStack<T>::push(std::shared_ptr<T> &&item) {
    std::shared_ptr<TStackItem<T>> other(new TStackItem<T>(item));
    other->SetNext(head);
    head = other;

}

template <class T> bool TStack<T>::empty() {
    return head == nullptr;
}

template <class T> std::shared_ptr<T> TStack<T>::pop() {
    std::shared_ptr<T> result;
    if (head != nullptr) {
        result = head->GetValue();
        head = head->GetNext();
    }

    return result;
}



template <class T> TStack<T>::~TStack() {

}

#endif	/* TSTACK_H */