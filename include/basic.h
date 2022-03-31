#ifndef __BASIC_H__
#define __BASIC_H__
#include<iostream>
#include<string.h>
#include<cstdlib>
#include<cstdio>
#include<regex>
#include<iomanip>
#include<unistd.h>
#include<sys/io.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<nedb.h>
using namespace std;

/*
    [C]Table - 数据表
    [C]Page - 数据页
    [C]Row - 数据行
    [C]Index - 动态索引
    [C]Executor - 命令执行器
    [C]Praser - 命令解析器
    [C]InputStream - 数据输入流
    [C]File - 数据存储与读入
    [C]NEexception - 全局异常处理器
    [C]BalanceTree - B+树
    [C]Implement - 工具类

    InputStream -> Praser(input) -> Executor(command)
    -> Table(command) -> Page(command) ->Row(command)
                        |
                        V
                 NEexception(error)
*/
/*
    DATA_TYPE  @  数据库支持的数据类型
    COMMAND  @  元命令类型
    OPERATION  @  操作命令类型
    Split(string str,char c,int& length)  @  字符串分割函数
    Table  @  数据表类 
    Page @  单页数据类
    Row  @  单行数据(数据基类)  

*/
#define TEXT_LENGTH 32
#define LONGTEXT_LENGTH 255
#define PAGE_HEAD_SIZE 40 
#define DATA_OFFSET 2

enum DATA_TYPE: __uint16_t{
    __INT,
    __INT64,
    __REAL,
    __TEXT,
    __LONGTEXT
};
enum COMMAND: __uint16_t{
    __UNKNOWN,
    __EXIT,
    __HELP,
    __OPERATION,
    __SAVE,
    __LOAD,
    __LOADALL
};
enum OPERATION: __uint16_t{
    CREATE_TABLE,
    CREATE_INDEX,
    INSERT_VALUES,
    DELETE_VALUES,
    UPDATE_VALUES,
    SELECT_VALUES,
    SELECT_TABLES,
    DESCRIBE_TABLE,
    DROP_TABLE,
    DROP_INDEX,
    UNDEFINED
};

enum NEexception:__uint16_t{
    //system
    SYSTEM_ERROR,
    //dir error
    HOME_DIR_ERROR,
    //file error
    FILE_OPEN_ERROR, FILE_NOT_FOUND, FILE_DAMAGED,
    //sql error
    SQL_FORM_ERROR, SQL_UNDEFINED,
    //table error
    TABLE_EXIST, TABLE_NOT_FOUND, TABLE_NUM_REACH_LIMIT,
    //parm error
    PARAM_EMPTY, PARAM_FORM_ERROR, PARAM_NOT_FOUND, PARAM_NUM_MISMATCH,
    VALUE_EMPTY,
    //nedb type error
    TYPE_UNDEFINED,
    TYPE_INT_MISMATCH, TYPE_INT64_MISMATCH, TYPE_REAL_MISMATCH,
    TYPE_INT_OVERFLOW, TYPE_INT64_OVERFLOW, TYPE_REAL_OVERFLOW,
    TYPE_TEXT_OVERFLOW, TYPE_LONGTEXT_OVERFLOW,
    //prim key error
    KEY_TYPE_NOT_ALLOWED,
    KEY_VAL_EXIST, KEY_VAL_REQUIRED, KEY_VAL_CHANGE_NOT_ALLOWED,
    //data error
    DATA_NOT_FOUND,
    //nedb operator charactor error
    COMPARE_NONSUPPORT,
};

union INDEX{
    int i_index;
    long long l_index;
    double d_index;
    char t_index[32];
};

class InputStream;
class Parser;
class Executor;
class Table;
class Page;
class Row;
class Analyzer;
class File;
class Implement;
class Memorizer;

extern string const NEexceptionName[];
extern string const kTypeName[];
extern int const kTypeSize[];
extern Table **__TABLES__;
extern int __CURSOR__;
//extern string __HomeDir__;
extern string __FramSuffix__;
extern string __DataSuffix__;
extern string __IndexSuffix__;

void __DATABASE_INIT__();
void __LOAD_ALL__(Table **, int &);
void __LOAD_FILE__(Table **, int &, string);
int __TABLE_LOCATED_BY_NAME__(Table **, int, string);
void __START__();
void __MESSAGE__();
void __HELP__();

#endif
