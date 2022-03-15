#ifndef __PROCESS_H__
#define __PROCESS_H__
#include <iostream>
#include "DataBase.h"

class InputStream{
private:
    __OPERATE__  __Operate;   //操作
    string __Statement;   //内容
    string __Input;   //所有输入
    /*
        元命令/操作命令解析
    */
    __COMMAND__  Parser(string input);
    __OPERATE__  Instruct(string input);
public:
    //初始化
    InputStream();
    //读入函数
    __COMMAND__ Read();
    string getInput();
    string getStatement();
    void ClearStatement();
    __OPERATE__ getOperation();
};

class Executor{
private:
    string __Statement;
    DataBase* __DataBase;
    int __Index_front;
    int __Index_tail;
    string __Name;
    void execute_create_table();
    void execute_insert_row();
    void execute_print_table();
    void execute_delete_row();
    void execute_remove_table();
public:
    Executor(DataBase* database);
    void Execute(__OPERATE__ operate,string statement);
    bool Analyse(string left);
};

#endif