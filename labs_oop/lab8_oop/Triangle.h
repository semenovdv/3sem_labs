#include <memory>
#include <cstdlib>
#include <iostream>
#include <math.h>




class Triangle{
public:
    Triangle();
    Triangle(std::istream &is);
    Triangle(size_t i,size_t j,size_t k);
    Triangle(const Triangle& orig);
    //friend std::ostream& operator<<(std::ostream& os, const Triangle& obj);

    double Square() const ;
    void   Print() const ;

    virtual ~Triangle();
private:
    size_t side_a;
    size_t side_b;
    size_t side_c;
};


Triangle::Triangle() : Triangle(0, 0, 0) {
}

Triangle::Triangle(size_t i, size_t j, size_t k) : side_a(i), side_b(j), side_c(k) {
    //std::cout << "Triangle created: " << side_a << ", " << side_b << ", " << side_c << std::endl;
}

Triangle::Triangle(std::istream &is) {
    is >> side_a;
    is >> side_b;
    is >> side_c;
    //std::cout << "Triangle created: " << side_a << ", " << side_b << ", " << side_c << std::endl;
}



double Triangle::Square() const {
    double p = double(side_a + side_b + side_c) / 2.0;
    return sqrt(p * (p - double(side_a))*(p - double(side_b))*(p - double(side_c)));

}

void Triangle::Print() const {
    std::cout << "a=" << side_a << ", b=" << side_b << ", c=" << side_c ;
}

std::ostream& operator<<(std::ostream& os, const Triangle& obj){
    obj.Print();
    return os;
}


Triangle::~Triangle() {
    //std::cout << "Triangle deleted" << std::endl;
}


