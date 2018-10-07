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
    int HIT;

    virtual void Init(const Data& from, const Data& to) = 0;
    virtual void Check(HashMap mp) = 0;
    virtual ~Abstract(){}

    void print_are(double num){
        cout << name << "," << num << "," << are << endl;
    }
    void print_aae(double num){
        cout << name << "," << num << "," << aae << endl;
    }
    void print_pr(double num){
        cout << name << "," << num << "," << pr << endl;
    }
    void print_cr(double num){
        cout << name << "," << num << "," << cr << endl;
    }
};

#endif // ABSTRACT_H
