#ifndef __BASIC_H__
#define __BASIC_H__
#include<iostream>
#include<string.h>
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
#define MAX_ROWS_SINGLE_PAGE 5
#define MAX_PAGES_SINGLE_TABLE 100
#define MAX_TABLES 50
enum __DataType__{__Short,__Int,__UInt,__Char,__VarChar11,__VarChar32,__VarChar255};
enum __COMMAND__{__UNK,__EXIT,__HELP,__OPERATE,__SAVE};
enum __OPERATE__{__UNKNOWN,__TABLE_CREATE,__INSERT,__DELETE,__REPLACE,__PRINT,
                 __TABLE_REMOVE,__SHOW};

class InputStream;
class Parser;
class Executor;
class Table;
class File;
class Exception;
class BalanceTree;
class Implement;

extern Table** __TABLES__;
extern int __CURSOR__;
void __DATABASE_INIT__();
int __TABLE_LOCATED_BY_NAME__(string name);
void __START__();

#endif
