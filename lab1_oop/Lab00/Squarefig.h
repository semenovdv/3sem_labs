/* 
 * File:   Squarefig.h
 * Author: semenov-dv-mai
 *
 * Created on Sep 14, 2018, 7:40 PM
 */

#ifndef SQUAREFIG_H
#define	SQUAREFIG_H
#include <cstdlib>
#include <iostream>
#include "Figure.h"

class Squarefig : public Figure{
public:
    Squarefig();
    Squarefig(std::istream &is);
    Squarefig(size_t a);
    Squarefig(const Squarefig& orig);

    double Square() override;
    void   Print() override;

    virtual ~Squarefig();
private:
    size_t side_a;

};

#endif	/* SQUAREFIG_H */

