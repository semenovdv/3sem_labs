#include <cstdlib>
#include <iostream>
#include <memory>


#include "Triangle.h"
#include "Octagon.h"
#include "Squarefig.h"

#include "TBTree.h"


int main(int argc, char const *argv[])
{
     
    TBTree<Triangle> ttree;
    ttree.Push(std::shared_ptr<Triangle>(new Triangle(1,1,1)));
    ttree.Push(std::shared_ptr<Triangle>(new Triangle(2,2,2)));
    ttree.Push(std::shared_ptr<Triangle>(new Triangle(3,3,3)));


    for(auto i : ttree){
        std::cout << *i << std::endl;
    }  
   
    
    TBTree<Octagon> otree;
    otree.Push(std::shared_ptr<Octagon>(new Octagon(1)));
    otree.Push(std::shared_ptr<Octagon>(new Octagon(2)));
    otree.Push(std::shared_ptr<Octagon>(new Octagon(3)));
    
    for(auto i : otree)  std::cout << *i << std::endl;
    
    TBTree<Squarefig> stree;
    stree.Push(std::shared_ptr<Squarefig>(new Squarefig(4)));
    stree.Push(std::shared_ptr<Squarefig>(new Squarefig(8)));
    stree.Push(std::shared_ptr<Squarefig>(new Squarefig(6)));

    for(auto i : stree)  std::cout << *i << std::endl;
    


    
    
    return 0;
}
