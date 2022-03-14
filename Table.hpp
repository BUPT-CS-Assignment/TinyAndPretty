#ifndef __TABLE__
#define __TABLE__
#include<iostream>
using namespace std;

template <class T>
class Table{
private:
    T *__Row;
public:
    int __ID;
    string __name;
    Table();
    void insert(T __T);
    void delete();
    void search();

};


#endif