#ifndef __TABLE_HPP__
#define __TABLE_HPP__

#include<iostream>
#include<string.h>

using namespace std;
enum __DataType__{__Int,__VarChar32,__VarChar255};
string* split(string str,char c,int &length);
class Table;
class Page;

class Row{
private:
    int ID;
    void** content;
public:
    Row(Table table);
    void padding(Table table,string statement);
    char* format(Table table);
    void erase(Table table);
    
};

class Page{
private:
    //int __ID;
    int __Folio;  //页码
    int __Index;
    int __MaxRows;
    int __Cursor;
    Row** __Rows;
public:
    Page(int folio,int max_rows);
    bool IsFull();
    bool getCursor();
    void Insert(Row* Node);
    void Delete(int row_number);
    void Remove();
    void Print(Table table);

};

class Table{
private:
    //int __Index;
    int __ID;
    int __Pagination;  //页数
    int __Cursor;
    string __Name;
    Page** __Pages;
    __DataType__*  __ElementType;
    string* __ElementID;
    int __ElementNumber;
public:
    Table(int ID, string name);
    bool Init(string statement);
    void Insert(string statement);
    void Print();
    void AddPage(Page* Node);
    void DeletePage(int page_number);
    void Remove();
    __DataType__* getElementType();
    int getElementNumber();
    string getName();
};


/*
{   
    char : xxx,
    varchar : xxx,
    ...;
}
*/
string* split(string str,char c,int &length){
    int count = 1;
    int index = 0;
	while ((index = str.find(c, index)) < str.length()){
		count++;
		index++;
	}
    string* words = new string[count];
    int start = 0, end = str.find(c, start);
    for(int i = 0;i<count;i++){
        words[i] = str.substr(start,end-start);
        start = end + 1;
        end = str.find(c,start);
        if(end == -1) end = str.length();
    }
    length = count;
    return words;
}

#endif