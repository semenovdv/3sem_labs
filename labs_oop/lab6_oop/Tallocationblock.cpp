#include "Tallocationblock.h"
#include <iostream>

Tallocationblock::Tallocationblock(size_t size,size_t count): _size(size),_count(count)  {
    _used_blocks = (char*)malloc(_size*_count);
    //_free_blocks = (void**)malloc(sizeof(void*)*_count);

    for(size_t i = 0; i < _count; i++) 
        free_block_tree.Insert((void**)(_used_blocks + i*_size));
    
    _free_count = _count;

    std::cout << "TAllocationBlock: Memory init" << std::endl;
}

void* Tallocationblock::allocate() {
    void* result = nullptr;
    
    if(_free_count>0)
    {
        result = free_block_tree.Searchlast();
        free_block_tree.Deletelast();
        _free_count--;
        std::cout << "TAllocationBlock: Allocate " << (_count-_free_count) << " of " << _count << std::endl;
    } else
    {
        std::cout << "TAllocationBlock: No memory exception :-)" << std::endl;
    }
    
    return result;
}

void Tallocationblock::deallocate(void* pointer) {
    std::cout << "TAllocationBlock: Deallocate block "<< std::endl;
    
    free_block_tree.Insert((void**)pointer);
  //_free_blocks[_free_count] = pointer;
  _free_count ++;

}

bool Tallocationblock::has_free_blocks() {
    return _free_count>0;
}

Tallocationblock::~Tallocationblock() {
    
    if(_free_count<_count) std::cout << "TAllocationBlock: Memory leak?" << std::endl;
                    else  std::cout << "TAllocationBlock: Memory freed" << std::endl;
    delete _used_blocks;
}