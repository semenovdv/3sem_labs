/* 
 * File:   Figure.h
 * Author: dvdemon
 *
 * Created on August 30, 2015, 6:33 PM
 */

#ifndef FIGURE_H
#define	FIGURE_H
#include <cstdlib>
#include <iostream>

class Figure {
public:
    Figure(){};
    virtual double Square() const = 0;
    virtual void   Print() const = 0;
    virtual ~Figure() {};
    friend std::ostream& operator<<(std::ostream& os, const Figure & fig){
        fig.Print();
        return os;
    }
};

#endif	/* FIGURE_H */

