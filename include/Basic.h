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
        [Row] - 数据行  
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
    __DataType__  @  数据库支持的数据类型
    __COMMAND__  @  元命令类型
    __OPERATE__  @  操作命令类型
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
#define BYTES_PER_PAGE 4000
#define PAGE_HEAD_SIZE 37
#define TABLE_UPDATE_OFFSET 38  
#define MAX_EMPTY_PAGE 20

enum __DataType__ : __uint16_t{__INT,__INT64,__REAL,__TEXT,__LONGTEXT};
enum __COMMAND__ : __uint16_t{__UNK,__EXIT,__HELP,__OPERATE,__SAVE,__LOAD,__LOADALL};
enum __OPERATE__ : __uint16_t{__UNKNOWN,__CREATE,__INSERT,__DELETE,__REPLACE,
                 __CHECK,__REMOVE,__SHOW,__DESCRIBE};

union __INDEX__{
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
class File;
class Exception;
class Implement;
class Memorizer;

extern string const __Type__[];
extern int const __TypeSize__ [];
extern Table** __TABLES__;
extern int __CURSOR__;
extern string __HOME_DIRECTORY__;
extern string __FILE_SUFFIX__;

void __DATABASE_INIT__();
int __TABLE_LOCATED_BY_NAME__(string name);
void __START__();
void __MESSAGE__();
void __HELP__();
void __LOAD_FILE__(string fileName);
void __LOAD_ALL_FILE__();


#endif
