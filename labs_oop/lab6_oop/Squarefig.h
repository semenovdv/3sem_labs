#include <memory>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include "Figure.h"

#include <iostream>
#include <math.h>


class Squarefig : public Figure{
public:
    Squarefig();
    Squarefig(std::istream &is);
    Squarefig(size_t a);
    Squarefig(const Squarefig& orig);
   // friend std::ostream& operator<<(std::ostream& os, const Squarefig& obj);

    double Square() const  override;
    void   Print() const override;

    virtual ~Squarefig();
private:
    size_t side_a;

};


Squarefig::Squarefig() : Squarefig(0) {
}

Squarefig::Squarefig(size_t a) : side_a(a) {
    //std::cout << "Square created: "  << side_a << std::endl;
}

Squarefig::Squarefig(std::istream &is) {
    is >> side_a;
    //std::cout << "Square created: "  << side_a << std::endl;
}



double Squarefig::Square() const {
    return side_a*side_a;

}

void Squarefig::Print() const {
    std::cout << "side_a=" << side_a ;

}

Squarefig::~Squarefig() {
    //std::cout << "Square deleted" << std::endl;
}

