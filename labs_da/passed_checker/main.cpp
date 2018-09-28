#include "vector.cpp"
#include "KeyValue.h"
#include <cmath>
#include <limits.h>
#include <iostream>      

void SortMatrix()


int main(){
    std::cout.precision(20); 
    long double key;
    std::string value;
    example::vector<KeyValue> input;

    while(std::cin >> key >> value){
        if(input.push_back(KeyValue(key, value)) == false){
            std::cout << "ERROR push_back" << std::endl;
            return 0;
        }
    }

    const long double  num_of_el = input.size();

    example::vector<example::vector<KeyValue>> matrix;
    matrix.resize(num_of_el);

    // making counting - filling matrix
    for(long long  i = 0; i < num_of_el; ++i){
        long double tmp = (long double)input[i].m_key / ((long double)ULLONG_MAX + 10);
        if(matrix[(long long)((num_of_el * tmp ))].push_back(input[i]) == false){
            std::cout << "ERROR push_back" << std::endl;
            return 0;
        }
    }   
    

    for(long long i = 0; i < matrix.size(); i++){
        if(matrix[i].size() != 0) {
            for(long long k = 1; k  < matrix[i].size(); k++){
            KeyValue tmp;
            tmp.m_key = matrix[i][k].m_key;
            tmp.m_value = matrix[i][k].m_value;
            long long j = k-1;
            while(j >= 0 && matrix[i][j].m_key > tmp.m_key){
                matrix[i][j+1].m_key = matrix[i][j].m_key;
                matrix[i][j+1].m_value = matrix[i][j].m_value;
                matrix[i][j].m_key = tmp.m_key;
                matrix[i][j].m_value = tmp.m_value;
                j--;
            }
            }

        }
        
    }

    for(unsigned long long i = 0; i < num_of_el; i++){
        if(matrix[i].size() != 0){
            for(unsigned long long j = 0; j < matrix[i].size(); j++){
                std::cout << matrix[i][j].m_key << "\t" << matrix[i][j].m_value << std::endl;
            }
        }
    }
    
}
