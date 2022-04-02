#ifndef __PROCESS_H__
#define __PROCESS_H__
#include <iostream>
#include<basic.h>

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
    NEDB* db_;
    Parser* parser_;
    void execute_create_table();
    void execute_insert_values();
    void execute_delete_values();
    void execute_select_values();
    void execute_update_values();
    void execute_describe_table();
    void execute_drop_table();
    void execute_create_index();
    void execute_drop_index();
    void execute_select_tables();
    ////////////////////////////////
    //void execute_print_table(Table*);
public:
    Executor(Parser*,NEDB*);
    void execute_command();
    void execute_operation();
};

#endif