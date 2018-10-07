#ifndef ABSTRACT_H
#define ABSTRACT_H

#include "data.h"
#include <iostream>

class Abstract{
public:
    string name;
    double pr;
    double cr;

    virtual void Init(const Data& data) = 0;
    virtual int Query(const Data &data, HashMap &mp) = 0;
    virtual void Check(HashMap mp, Abstract* another) = 0;
    virtual ~Abstract(){};

    void print_pr(double num){
        cout << name << "," << num << "," << pr << endl;
    }
    void print_cr(double num){
        cout << name << "," << num << "," << cr << endl;
    }
};

#endif // ABSTRACT_H
