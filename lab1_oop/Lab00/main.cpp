/* 
 * File:   main.cpp
 * Author: dvdemon
 *
 * Created on August 11, 2015, 12:40 PM
 */

#include <cstdlib>
#include "Triangle.h"
#include "Octagon.h"
#include "Squarefig.h"

int main(int argc, char** argv) {

    Figure *ptr1 = new Triangle(std::cin);
    ptr1->Print();
    std::cout << ptr1->Square() << std::endl;
    delete ptr1;

    Figure *ptr2 = new Squarefig(std::cin);
    ptr2->Print();
    std::cout << ptr2->Square() << std::endl;
    delete ptr2;

    Figure *ptr3 = new Octagon(std::cin);
    ptr3->Print();
    std::cout << ptr3->Square() << std::endl;
    delete ptr3;


    return 0;
}

