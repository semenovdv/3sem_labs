/* 
 * File:   Squarefig.cpp
 * Author: semenov-dv-mai
 * 
 * Created on Sep 14, 2018, 7:52 PM
 */

#include "Squarefig.h"
#include <iostream>
#include <cmath>

Squarefig::Squarefig() : Squarefig(0) {
}

Squarefig::Squarefig(size_t a) : side_a(a) {
    std::cout << "Square created: "  << side_a << std::endl;
}

Squarefig::Squarefig(std::istream &is) {
    is >> side_a;
    std::cout << "Square created: "  << side_a << std::endl;
}



double Squarefig::Square() const {
    return side_a*side_a;

}

void Squarefig::Print() const {
    std::cout << "side_a=" << side_a ;

}

Squarefig::~Squarefig() {
    std::cout << "Square deleted" << std::endl;
}


