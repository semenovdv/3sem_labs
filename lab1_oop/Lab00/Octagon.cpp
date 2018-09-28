/* 
 * File:   Octagon.cpp
 * Author: semenov-dv-mai
 * 
 * Created on Sep 14, 2018, 7:52 PM
 */

#include "Octagon.h"
#include <iostream>
#include <cmath>
#include <vector>


Octagon::Octagon() : Octagon(0) {
}

Octagon::Octagon(size_t a) : 
side_a(a)  {
    std::cout << "Octagon created: " 
    << side_a << std::endl;
}

Octagon::Octagon(std::istream &is) {
    is >> side_a;
}

Octagon::Octagon(const Octagon& orig) {
    std::cout << "Octagon copy created" << std::endl;
    side_a = orig.side_a;
    
}
//TODO Узнать площадь 8-уг-ка
double Octagon::Square() {
    return 2*side_a*side_a*(1+sqrt(2));
}

void Octagon::Print() {
    for(int i = 0; i < 8; i++){
        std::cout << "a=" << side_a << std::endl;
    }

}

Octagon::~Octagon() {
    std::cout << "Octagon deleted" << std::endl;
}


