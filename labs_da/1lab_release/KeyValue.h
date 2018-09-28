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
    long double m_key; // Ключ
    std::string m_value; //Значение

    KeyValue() {}
    
    KeyValue(long double key, std::string value){
        m_value = value;
        m_key = key;
    }

    ~KeyValue(){
    }

};

#endif	/* KEYVALUE_H */