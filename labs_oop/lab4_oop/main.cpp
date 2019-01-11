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
    std::cout << ttree << std::endl;

    TBTree<Octagon> otree;
    otree.Push(std::shared_ptr<Octagon>(new Octagon(1)));
    otree.Push(std::shared_ptr<Octagon>(new Octagon(2)));
    otree.Push(std::shared_ptr<Octagon>(new Octagon(3)));
    std::cout << otree << std::endl;

    TBTree<Squarefig> stree;
    stree.Push(std::shared_ptr<Squarefig>(new Squarefig(4)));
    stree.Push(std::shared_ptr<Squarefig>(new Squarefig(5)));
    stree.Push(std::shared_ptr<Squarefig>(new Squarefig(6)));
    std::cout << stree << std::endl;



    
    
    return 0;
}
