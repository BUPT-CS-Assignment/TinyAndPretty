#ifndef __PROCESS_H__
#define __PROCESS_H__
#include <main.h>
#include <Interface/NEDB.h>

int __LockCheck__(int& lock,int times);

class InputStream{
private:
    std::string     __Input;
public:
    InputStream();
    std::string     read();
    std::string     getInput();
    void            clear_input();
};

class Parser{
friend class Executor;
private:
    std::string statement_;
    std::string object_;
    std::string condition_;
    std::string value_;
    COMMAND     command_;
    OPERATION   operation_;
    /////////////////////////////////////
    OPERATION   operate_type(std::string input);
    void        deconstruct();
    void        parser_create_table();
    void        parser_insert_values();
    void        parser_delete_values();
    void        parser_select_values();
    void        parser_update_values();
    void        parser_describe_table();
    void        parser_create_index();
    void        parser_drop_index();
    void        parser_drop_table();
public:
    Parser();
    void        i_analyse(std::string input);
    void        analyse(std::string input);
    COMMAND     getCommand();
    OPERATION   getOperate();
    std::string getStatment();
    void        flush();

};

class Executor{
private:
    DataBase*   db_;
    Parser*     parser_;
    void        execute_create_table();
    void        execute_insert_values();
    void        execute_delete_values();
    void        execute_select_values();
    void        execute_update_values();
    void        execute_describe_table();
    void        execute_drop_table();
    void        execute_create_index();
    void        execute_drop_index();
    void        execute_select_tables();
    ////////////////////////////////
public:
    Executor(Parser*,DataBase*);
    void        execute_command();
    void        execute_operation();
};

#endif