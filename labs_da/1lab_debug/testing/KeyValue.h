/* 
 * File:   KeyValue.h
 * Author: semenov-dv-mai
 *
 * Created on Sep 16, 2015, 3:40 PM
 */

#ifndef KEYVALUE_H
#define	KEYVALUE_H
#include <iostream>

class KeyValue {
public:
    long double m_key;
    std::string m_value;
    //char m_value[2049];


    KeyValue() {}
    
    KeyValue(unsigned long long key, std::string value){
        /*int length = 0;
        while(value[length++] != '\0');
        for(int i = 0; i < length; ++i){
            m_value[i] = value[i];
        } */
        m_value = value;
        m_key = key;
    }

    ~KeyValue(){
    }

};

#endif	/* KEYVALUE_H */