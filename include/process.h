#ifndef __PROCESS_H__
#define __PROCESS_H__
#include <iostream>
#include <basic.h>

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
friend class Executor;
private:
    string statement_;
    string object_;
    string condition_;
    string value_;
    //int __Limit;
    //
    COMMAND  command_;
    OPERATION  operation_;
    OPERATION operate_type(string input);
    void deconstruct();
    void parser_create_table();
    void parser_insert_values();
    void parser_delete_values();
    void parser_select_values();
    void parser_update_values();
    void parser_describe_table();
    void parser_create_index();
    void parser_drop_index();
    void parser_drop_table();
public:
    Parser();
    void i_analyse(string input);
    void analyse(string input);
    COMMAND getCommand();
    OPERATION getOperate();
    string getStatment();
    void flush();

};

class Executor{
private:
    Parser* parser_;
    void execute_create_table(Table**,int&);
    void execute_insert_values(Table**,int);
    void execute_delete_values(Table**,int);
    void execute_select_values(Table**,int,string&);
    void execute_update_values(Table**,int);
    void execute_describe_table(Table**,int,string&);
    void execute_drop_table(Table**,int&);
    void execute_create_index(Table**,int);
    void execute_drop_index(Table**,int);
    ////////////////////////////////
    //void execute_print_table(Table*);
public:
    Executor(Parser*);
    void execute_command();
    void execute_operation(Table**,int&,string&);
};

#endif