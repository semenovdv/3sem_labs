#include <cstdlib>
#include <iostream>
#include <memory>
#include "Triangle.h"
#include "TBTree.h"
#include <random>
#include <chrono>


using namespace std;



int main(int argc, char const *argv[])
{

    TBTree<Triangle> tree;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1, 100);

    for (int i = 0; i < 500; i++) {
        int side = distribution(generator);
        tree.Push(new Triangle(side, side, side));
    }

    
    std::cout << "DFS -------------" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    //stack.sort();
    tree.dfs_parallel(tree.root);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Done -------------" << std::endl;
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << std::endl;

    
    return 0;
}
