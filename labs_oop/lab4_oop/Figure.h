#include <memory>

#include <cstdlib>
#include <iostream>
#include <math.h>

#ifndef FIGURE_H
#define FIGURE_H

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

#endif
