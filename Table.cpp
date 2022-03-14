#include<iostream>
#include<string.h>
#include "Table.hpp"
using namespace std;

//enum __DataType__{__Int,__VarChar32,__VarChar255};
//string* split(string str,char c,int &length);


Row::Row(Table table){
    __DataType__* element_type = table.getElementType();
    int element_number = table.getElementNumber();
    content = new void*[element_number];
    for(int i = 0;i<element_number;i++){
        if(element_type[i] == __VarChar32){
            content[i] = new char[32];
        }else if(element_type[i] == __VarChar255){
            content[i] = new char[255];
        }else if(element_type[i] == __Int){
            content[i] = new int;
        }
    }
}
    
void Row::padding(Table table,string statement){
    int l = 0;
    string* words = split(statement,',',l); 
    int element_number = table.getElementNumber();
    if(l != element_number){
        cout<<"NOT MATCH!\n"<<endl;
        return;
    }
    for(int i = 0;i<element_number;i++){
        if(table.getElementType()[i] == __Int){
            int number = stoi(words[i].c_str());
            *((int*)content[i]) = number;
            //scanf("%d",content[i]);
        }else{
            char* temp = (char*)content[i];
            strcpy(temp,words[i].c_str());
            //strcpy((char*)content[i],temp);
            //scanf("%s",content[i]);
        }
    }
}  

char* Row::format(Table table){
    int element_number = table.getElementNumber();
    string temp = "";
    for(int i = 0;i<element_number;i++){
        if(table.getElementType()[i] == __Int){
            temp = temp + to_string(*((int*)content[i]));
        }else{
            temp = temp + (char*)content[i];
        }
        temp = temp + (i==element_number-1?"\n":"\t");
    }
    char* row = new char[temp.length()+1];
    strcpy(row,temp.c_str());
    return row;
}

void Row::erase(Table table){
    int element_number = table.getElementNumber();
    for(int i = 0;i<element_number;i++){
        //delete[] content[i];
    }
}
    

Page::Page(int folio,int max_rows = 20){
    __Folio = folio;
    __MaxRows = max_rows;
    __Rows = new Row*[__MaxRows];
    __Index = 0;
    __Cursor = 0;
}   
bool Page::IsFull(){
    return __Cursor >= __MaxRows;
}
bool Page::getCursor(){
    return __Cursor;
}
void Page::Insert(Row* Node){
    __Rows[__Cursor ++ ] = Node;
}
void Page::Delete(int row_number){
    delete[] __Rows[row_number];
    for(int i = row_number+1;i<__Cursor;i++){
        __Rows[i-1] = __Rows[i];
    }
    __Cursor--;
}
void Page::Remove(){
    for(int i = 0;i<__Cursor;i++){
        delete[] __Rows[i];
    }
}
void Page::Print(Table table){
    for(int i = 0;i<__Cursor;i++){
        cout<<"["<<i<<"]\t"<<__Rows[i]->format(table);
    }
    for(int i = 0;i<table.getElementNumber();i++){
        cout<<"\t";
    }cout<<"[Page : "<<__Folio<<"]\t\n"<<endl;
}



Table::Table(int ID, string name = "New Table"){
    __ID = ID;
    __Pagination = 0;
    __Cursor = 0;
    __ElementNumber = 0;
    __Name = name;
    __Pages = new Page*[100];
}

bool Table::Init(string statement){
    string* line = split(statement,',',__ElementNumber);
    __ElementType = new __DataType__[__ElementNumber];
    __ElementID = new string[__ElementNumber];
    cout<<statement<<endl;
    for(int i =0;i<__ElementNumber;i++){
        string temp = line[i];
        int index= temp.find(':');
        __ElementID[i] = temp.substr(0,index);
        string elementType = temp.substr(index+1,temp.length()-index);
        if(elementType.compare("int") == 0){
            __ElementType[i] = __Int;
        }else if(elementType.compare("varchar32") == 0){
            __ElementType[i] = __VarChar32;
        }else if(elementType.compare("varchar255") == 0){
            __ElementType[i] = __VarChar255;
        }else{
            cout<<"Unknown Element Type."<<endl;
            Remove();
            return false;
        }
    }
    return true;
}

void Table::Insert(string statement){
    Row* new_row = new Row(*this);
    new_row->padding(*this,statement);
    if(__Cursor == 0 || __Pages[__Cursor - 1]->IsFull()){
        __Pages[__Cursor] = new Page(__Cursor);
        __Cursor++;
    }
    __Pages[__Cursor-1]->Insert(new_row);
}


void Table::Print(){
    cout<<"\t";
    for(int i = 0;i<__ElementNumber;i++){
        cout<<"["<<__ElementID[i]<<"]\t";
    }cout<<endl;
    for(int i =0;i<__Cursor;i++){
        __Pages[i]->Print(*this);
    }
}

void Table::AddPage(Page* Node){
    __Pages[__Cursor ++ ] = Node;
    __Pagination ++;
}

void Table::DeletePage(int page_number){
    delete[] __Pages[page_number];
    for(int i = page_number+1;i<__Cursor;i++){
        __Pages[i-1] = __Pages[i];
    }
    __Cursor--;
}

void Table::Remove(){
    for(int i = 0;i<__Cursor;i++){
        delete[] __Pages[i];
    }
    delete[] __ElementType;
    delete[] __ElementID;
}

__DataType__* Table::getElementType(){
    return __ElementType;
}

int Table::getElementNumber(){
    return __ElementNumber;
}

string Table::getName(){
    return __Name;
}




/*
{   
    char : xxx,
    varchar : xxx,
    ...;
}
*/










