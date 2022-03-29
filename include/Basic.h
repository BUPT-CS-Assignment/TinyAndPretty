#ifndef __BASIC_H__
#define __BASIC_H__
#include<iostream>
#include<string.h>
#include<cstdlib>
#include<cstdio>
#include<regex>
#include<iomanip>
#include<unistd.h>
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
    [C]Exception - 全局异常处理器
    [C]BalanceTree - B+树
    [C]Implement - 工具类

    InputStream -> Praser(input) -> Executor(command)
    -> Table(command) -> Page(command) ->Row(command)
                        |
                        V
                 Exception(error)
*/
/*
    DATA_TYPE  @  数据库支持的数据类型
    COMMAND  @  元命令类型
    OPERATION  @  操作命令类型
    Split(string str,char c,int& length)  @  字符串分割函数
    Table  @  数据表类 
    Page @  单页数据类
    Row  @  单行数据类(数据基类)  

*/
#define MAX_ROWS_SINGLE_PAGE 20
#define MAX_PAGES_SINGLE_TABLE 100
#define MAX_TABLES 50
#define TEXT_LENGTH 32
#define LONGTEXT_LENGTH 255
#define PAGE_SIZE 200
#define PAGE_HEAD_SIZE 40 
#define MAX_EMPTY_PAGE 20
#define DATA_OFFSET 2

enum DATA_TYPE : __uint16_t{
                __INT,
                __INT64,
                __REAL,
                __TEXT,
                __LONGTEXT
                };
enum COMMAND : __uint16_t{
                __UNKNOWN,
                __EXIT,
                __HELP,
                __OPERATION,
                __SAVE,
                __LOAD,
                __LOADALL
                };
enum OPERATION : __uint16_t{
                CREATE_TABLE,
                CREATE_INDEX,
                INSERT_VALUES,
                DELETE_VALUES,
                UPDATE_VALUES,
                SELECT_VALUES,
                DESCRIBE_TABLE,
                DROP_TABLE,
                DROP_INDEX,
                UNDEFINED
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
class Exception;
class Implement;
class Memorizer;

extern string const kTypeName[];
extern int const kTypeSize [];
extern Table** __TABLES__;
extern int __CURSOR__;
extern string kHomeDir;
extern string kFramSuffix;
extern string kDataSuffix;
extern string kIndexSuffix;

void __DATABASE_INIT__();
int __TABLE_LOCATED_BY_NAME__(string name);
void __START__();
void __MESSAGE__();
void __HELP__();
void __LOAD_FILE__(string fileName);
void __LOAD_ALL_FILE__();


#endif
