#include <cstdlib>
#include <iostream>

#include "Octagon.h"
#include "Triangle.h"
#include "Squarefig.h"
#include "TNode.h"
#include "TBTree.h"
#include "Figure.h"

int main(int argc, char const *argv[])
{
    std::cout << "enter number of elements" << std::endl;
    int n;
    std::cin >> n; 
    TBTree tree = TBTree();

    for(int i = 0; i < n; i++){
        std::cout << "Введите 0 для треугольника\n 1 для восьмиугольника\n и 2 для квадрата\n -1 для выхода\n";
        int a;
        std::cin >> a;
        switch(a) {
        case -1:{
            std::cout << "exit" << std::endl;
            return 0;
        }
        case 0: {
            std::cout << "Введите сторны треугольника: " << std::endl;
            int s1,s2,s3;
            std::cin >> s1 >> s2 >> s3;
            tree.Push(std::shared_ptr<Triangle>(new Triangle(s1, s2, s3)));
            break;
        }
        case 1:{
            std::cout << "Введите сторону восьмиугольника: " << std::endl;
            int s;
            std::cin >> s;
            tree.Push(std::shared_ptr<Octagon>(new Octagon(s)));
            break;
        }
        case 2:{
            std::cout << "Введите сторону квадрата: " << std::endl;
            int s;
            std::cin >> s;
            tree.Push(std::shared_ptr<Squarefig>(new Squarefig(s)));
            break;
        }
        default:{
            std::cout << "No such figure " << std::endl;
        }
        }
        std::cout << tree << std::endl;
    }
    
    return 0;
}
