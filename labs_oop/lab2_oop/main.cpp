#include <cstdlib>
#include <iostream>

#include "Octagon.h"
#include "TNode.h"
#include "TBTree.h"

int main(int argc, char const *argv[])
{
    std::cout << "enter number of elements" << std::endl;
    int n;
    std::cin >> n; 
    TBTree tree = TBTree();

    for(int i = 0; i < n; i++){
        Octagon a;
        std::cin >> a;
        tree.Push(a);
        std::cout << tree << std::endl;
    }
    
    return 0;
}
