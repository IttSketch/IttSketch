#ifndef ABSTRACT_H
#define ABSTRACT_H

#include "data.h"
#include <iostream>

class Abstract{
public:
    string name;
    double aae;
    double are;
    double pr;
    double cr;

    virtual void Init(const Data& data) = 0;
    virtual void Check(HashMap mp) = 0;
    virtual ~Abstract(){};

    void print_are(int num){
        cout << name << "," << num << "," << are << endl;
    }
    void print_aae(int num){
        cout << name << "," << num << "," << aae << endl;
    }
    void print_pr(int num){
        cout << name << "," << num << "," << pr << endl;
    }
    void print_cr(int num){
        cout << name << "," << num << "," << cr << endl;
    }
};

#endif // ABSTRACT_H
