#ifndef __PROCESS_H__
#define __PROCESS_H__
#include <main.h>


class NEDBnamespace::InputStream{
private:
    std::string     __Input;
public:
    InputStream();
    std::string     read();
    std::string     getInput();
    void            clear_input();
};

class NEDBnamespace::Parser{
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
    void        parser_drop_table();
public:
    Parser();
    void        i_analyse(std::string input);
    void        analyse(std::string input);
    void        setStatement(std::string input);
    void        setObject(std::string input);
    void        setCondition(std::string input);
    void        setValue(std::string input);
    void        setOperation(OPERATION input);
    COMMAND     getCommand();
    OPERATION   getOperate();
    std::string getStatment();
    void        flush();

};

class NEDBnamespace::Executor{
private:
    NEDBSTD::DataBase*   db_;
    Parser*     parser_;
    void        execute_create_table();
    void        execute_insert_values();
    void        execute_delete_values();
    void        execute_select_values();
    void        execute_update_values();
    void        execute_describe_table();
    void        execute_drop_table();
    void        execute_select_tables();
    ////////////////////////////////
public:
    Executor(Parser*,NEDBSTD::DataBase*);
    int         count_;
    std::string returnValue_;
    int         errCode_;
    void        execute_command();
    void        execute_operation();
};

#endif