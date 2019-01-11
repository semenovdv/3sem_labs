/* 
 * File:   main.cpp
 * Author: dvdemon
 *
 * Created on August 11, 2015, 12:40 PM
 */

#include <cstdlib>
#include "Triangle.h"
#include "Octagon.h"
#include "Squarefig.h"
#include "TAllocationBlock.h"


int main(int argc, char** argv) {
    int fig = 5; //1 - triangle, 2 - square, 3 - octagon
    Figure *ptr;
    while(fig != 0){
        //считывание номера
        std::cout << "Введите номер фигуры (0 - выход, 1 - треугольник, 2 - квадрат, 3 - восьмиугольник): " << std::endl;
        std::cin >> fig;
       
        switch ( fig ) {
            case 1:
            {
                std::cout << "Введите стороны треугольника" << std::endl;
                ptr = new Triangle(std::cin);

                break;
            }
            case 2:
            { 
                std::cout << "Введите сторону квадрата" << std::endl;
                ptr = new Squarefig(std::cin);

                break;
            }
            case 3:
            { 
                std::cout << "Введите сторону правильного восьмиугольника" << std::endl;
                ptr = new Octagon(std::cin);

                break;
            }
            default:
                if(fig != 0) std::cout << "Number is wrong, try again" << std::endl;
        }
        //вызов функций для созданной фигуры
        if(fig != 0){
            ptr->Print();
            std::cout << ptr->Square() << std::endl;
        }    
    }
    delete ptr;

    return 0;
}

