#ifndef __PROCESS_H__
#define __PROCESS_H__
#include <iostream>
#include <Basic.h>

class InputStream{
private:
    string __Input;
public:
    InputStream();
    string read();
    string getInput();
    void clear_input();
};

class Parser{
private:
    string __Statement;
    __COMMAND__  __Command;
    __OPERATE__  __Operate;
    __OPERATE__ operate_type(string input);
public:
    Parser();
    void analyse(string input);
    __COMMAND__ getCommand();
    __OPERATE__ getOperate();
    string getStatment();

};

class Executor{
private:
    string __Statement;
    int __Index_Front;
    int __Index_Tail;
    string __Table_Name;
    string __Parameter;
    void execute_create_table();
    void execute_insert_row();
    void execute_print_table();
    void execute_delete_row();
    void execute_remove_table();
    void execute_show_tables();
    void execute_describe_table();
public:
    bool pre_process(string symbol);
    Executor();
    void execute(__COMMAND__ command,string statement);
    void execute(__OPERATE__ operate,string statement);

};

#endif