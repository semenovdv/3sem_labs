#include <memory>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include "Figure.h"


class Octagon : public Figure{
public:
    Octagon();
    Octagon(std::istream &is);
    Octagon(size_t a);
    Octagon(const Octagon& orig);

    double Square() const override;
    void   Print() const  override;

    friend std::istream& operator>>(std::istream& is, Octagon& obj);
    friend std::ostream& operator<<(std::ostream& os, const Octagon& obj);


    virtual ~Octagon();
private:
    size_t side_a;
};


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

std::ostream& operator<<(std::ostream& os, const Octagon& obj){
    obj.Print();
    return os;
}


Octagon::~Octagon() {
    //std::cout << "Octagon deleted" << std::endl;
}
