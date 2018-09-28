#include <limits.h>
#include <iostream>
#include <cmath>
#include "vector.cpp"
#include "KeyValue.h"
int main(){
        std::cout.precision(20);

    std::cout << 1/ULLONG_MAX << std::endl;
    std::cout << 1 + 1/ULLONG_MAX << std::endl;
    std::cout << (long double)ULLONG_MAX << std::endl;
    unsigned long long k = 0;
    std::cout << k-1 << std::endl;
    long double n;
    std::cin >> n;
    example::vector<long double> vect;
  
    long double m_key = ULLONG_MAX/n;
    std::cout << m_key << std::endl;
    long double res = 0;
    for(int i = 0; i < n; i++){
        std::cout << (unsigned long long)((res*n)/ULLONG_MAX)<< " " << floor(res*ULLONG_MAX) << std::endl;
        res+=m_key;
    }

    for(int i = 0; i < vect.size(); i++){
        std::cout << vect[i] << std::endl;
    }
} 