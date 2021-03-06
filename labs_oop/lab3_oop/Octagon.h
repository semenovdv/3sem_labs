/* 
 * File:   Octagon.h
 * Author: semenov-dv-mai
 *
 * Created on Sep 14, 2018, 7:40 PM
 */

#ifndef OCTAGON_H
#define	OCTAGON_H
#include <cstdlib>
#include <iostream>
#include "Figure.h"

class Octagon : public Figure{
public:
    Octagon();
    Octagon(std::istream &is);
    Octagon(size_t a);
    Octagon(const Octagon& orig);
    

    double Square() const override;
    void   Print() const override;

    friend std::istream& operator>>(std::istream& is, Octagon& obj);
    //friend std::ostream& operator<<(std::ostream& os, const Octagon& obj);


    virtual ~Octagon();
private:
    size_t side_a;
};

#endif	/* OCTAGON_H */

