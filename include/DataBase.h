#ifndef __DATABASE_H__
#define __DATABASE_H__
#include<iostream>
#include<string.h>
using namespace std;
/*
    __DataType__  @  数据库支持的数据类型
    __COMMAND__  @  元命令类型
    __OPERATE__  @  操作命令类型
    Split(string str,char c,int& length)  @  字符串分割函数
    Table  @  数据表类 
    Page @  单页数据类
    Row  @  单行数据类(数据基类)  

*/
#define MAX_ROWS_SINGLE_PAGE 5
enum __DataType__{__Int,__VarChar32,__VarChar255};
enum __COMMAND__{__UNK,__EXIT,__HELP,__OPERATE,__SAVE};
enum __OPERATE__{__UNKNOWN,__TABLE_CREATE,__INSERT,__DELETE,__REPLACE,__PRINT,
                 __TABLE_REMOVE,__SHOW};
class Table;
class Page;
class Row;
class InputStream;
class DataBase{
private:
    Table** __DataBase__;
    int __Cursor__;
public:
    Table** getDataBase(){
        return __DataBase__;
    }
    int getCursor(){
        return __Cursor__;
    }
    void AddTable(Table* table){
        __DataBase__[__Cursor__++] = table;
    }
    Table* RemoveTable(int index){
        Table* temp = __DataBase__[index];
        for(int i = index+1; i < __Cursor__; i++){
            __DataBase__[i-1] = __DataBase__[i];
        }
        __Cursor__ -- ;
        return temp;
    }
    int LocateByName(string name);
    DataBase();
    void Run();
};

#endif
