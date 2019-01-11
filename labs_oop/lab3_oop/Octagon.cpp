/* 
 * File:   Octagon.cpp
 * Author: semenov-dv-mai
 * 
 * Created on Sep 14, 2018, 7:52 PM
 */

#include "Figure.h"
#include "Octagon.h"
#include <iostream>
#include <cmath>
#include <vector>


Octagon::Octagon() : Octagon(0) {
}

Octagon::Octagon(size_t a) : 
side_a(a)  {
    //std::cout //<< "Octagon created: " 
    //<< side_a << std::endl;
}

Octagon::Octagon(std::istream &is) {
    is >> side_a;
    //std::cout << "Octagon created" << side_a << std::endl;
}


//TODO Узнать площадь 8-уг-ка
double Octagon::Square() const {
    return 2*side_a*side_a*(1+sqrt(2));
}

void Octagon::Print() const  {
    std::cout << "side_a=" << side_a ;
    

}

std::istream& operator>>(std::istream& is, Octagon& obj){
    is >> obj.side_a;

    return is;
}
/*
std::ostream& operator<<(std::ostream& os, const Octagon& obj){
    os << obj.side_a;
    return os;
}
*/

Octagon::~Octagon() {
    //std::cout << "Octagon deleted" << std::endl;
}


