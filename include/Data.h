#ifndef __DATA_H__
#define __DATA_H__
#include<iostream>
#include"DataBase.h"
using namespace std;

class Table{
private:
    //int __Index;
    int __ID;
    int __Cursor;
    string __Name;
    Page** __Pages;
    __DataType__*  __ElementType;
    string* __ElementID;
    int __ElementNumber;
public:
    Table(int ID, string name);
    bool Init(string statement);
    bool Insert(string statement);
    bool DeletePage(int page_number);
    bool DeleteRow(int page_number,int row_number);
    bool Remove();
    void Print();
    void AddPage(Page* Node);
    __DataType__* getElementType();
    int getElementNumber();
    string getName();
    Page* getPage(int page_number);
};

class Page{
private:
    //int __ID;
    int __Folio;  //页码
    int __Index;  //
    int __MaxRows;
    int __Cursor;
    Row** __Rows;
public:
    Page(int folio);
    bool IsFull();
    int getCursor();
    void Insert(Row* Node);
    bool Delete(int row_number);
    bool Remove();
    void Print(Table table);
};

class Row{
private:
    int ID;
    void** content;
public:
    Row(Table table);
    bool Padding(Table table,string statement);
    char* Format(Table table);
    void Erase(Table table);
};



#endif
