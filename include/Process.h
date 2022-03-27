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
    bool deconstruct();
    bool parser_create_table();
    bool parser_insert_values();
    bool parser_delete_values();
    bool parser_select_values();
    bool parser_update_values();
    bool parser_describe_table();
    bool parser_create_index();
    bool parser_drop_index();
    bool parser_drop_table();
public:
    Parser();
    void analyse(string input);
    COMMAND getCommand();
    OPERATION getOperate();
    string getStatment();
    void flush();

};

class Executor{
private:
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
    ////////////////////////////////
    void execute_print_table();
public:
    Executor(Parser*);
    void execute_command();
    void execute_operation();
};

#endif