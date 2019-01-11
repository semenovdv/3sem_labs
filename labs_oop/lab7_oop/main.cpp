#include <cstdlib>
#include <iostream>
#include <memory>

#include "Triangle.h"
#include "TBTree.h"
#include "NTree.h"

#include "IRemoveCriteriaByValue.h"
#include "IRemoveCriteriaAll.h"


int main(int argc, char const *argv[])
{
    TBTree<NTree<Triangle>, Triangle> tree;
    
    tree.InsertSubitem(new Triangle(1,1,1));
    tree.InsertSubitem(new Triangle(2,1,1));
    tree.InsertSubitem(new Triangle(3,1,1));
    tree.InsertSubitem(new Triangle(4,1,1));
    tree.InsertSubitem(new Triangle(5,1,1));
    tree.InsertSubitem(new Triangle(6,1,1));
    tree.InsertSubitem(new Triangle(7,1,1));
    int a;
    std::cin >> a;
    std::cout << tree << std::endl;
    
    IRemoveCriteriaByValue<Triangle> criteria(Triangle(4,1,1));
    IRemoveCriteriaAll<Triangle> criteriaAll;
    tree.RemoveSubitem(&criteria);
    
    
    
    std::cout << tree << std::endl;
    return 0;
}





