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
#include<NEdb.h>
using namespace std;

#define DEFAULT_PAGE_SIZE 400
#define MAX_TABLES       50
#define TEXT_LENGTH      32
#define LONGTEXT_LENGTH  255
#define PAGE_HEAD_SIZE   40 
#define DATA_OFFSET      2
#define __FramSuffix__   ".nef"
#define __DataSuffix__   ".ned"
#define __IndexSuffix__  ".nei"
#define __DefaultDir__   "/home/jianxf/.nesrc/"

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
    __SETDIR,
    __SHOWDIR,
    __DIRINIT,
    __LOAD,
    __LOADALL,
    __SETPAGESIZE,
    __SHOWPAGESIZE
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
    SYSTEM_ERROR, SIZE_NOT_ALLOWED,
    //dir error
    DIR_ERROR,
    //file error
    FILE_OPEN_ERROR, FILE_NOT_FOUND, FILE_DAMAGED,
    //sql error
    SQL_FORM_ERROR, SQL_UNDEFINED,
    //table error
    TABLE_EXIST, TABLE_NOT_FOUND, TABLE_NUM_REACH_LIMIT,
    //parm error
    PARAM_EMPTY, PARAM_FORM_ERROR, PARAM_NOT_FOUND, PARAM_NUM_MISMATCH,
    VALUE_EMPTY,
    //NEDB type error
    TYPE_UNDEFINED,
    TYPE_INT_MISMATCH, TYPE_INT64_MISMATCH, TYPE_REAL_MISMATCH,
    TYPE_INT_OVERFLOW, TYPE_INT64_OVERFLOW, TYPE_REAL_OVERFLOW,
    TYPE_TEXT_OVERFLOW, TYPE_LONGTEXT_OVERFLOW,
    //prim key error
    KEY_TYPE_NOT_ALLOWED,
    KEY_VAL_EXIST, KEY_VAL_REQUIRED, KEY_VAL_CHANGE_NOT_ALLOWED,
    //data error
    DATA_NOT_FOUND,
    //NEDB operator charactor error
    COMPARE_NONSUPPORT,
    //command error
    COMMAND_UNDEFINED
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

//extern string const NEexceptionName[];
//extern string const kTypeName[];
//extern int const kTypeSize[];


static string const kTypeName[] = {"int","int64","real","text","longtext"};
static int const kTypeSize[] = {4,8,8,32,255};
////////////////////////////////////////////////////////////////
static string const NEexceptionName[] = {
    //system
    "system error","size not allowed",
    //dir error
    "dir error",
    //file error
    "file open error", "file not found", "file damaged",
    //sql error
    "sql form mismatch", "sql undefined",
    //table error
    "table existed", "table not found", "table number reach limit",
    //parm error
    "param empty", "param form mismatch", "param not found", "param num mismatch",
    "value empty",
    //NEDB type error
    "type undefined",
    "type int mismatch", "type int64 mismatch", "type real mismatch",
    "type int overflow", "type int64 overflow", "type real overflow",
    "type text overflow", "type longtext overflow",
    //prim key error
    "key type not allowed",
    "key value existed", "key value required", "key value change not allowed",
    //data error
    "data not found",
    //NEDB operator charactor error
    "compare nonsupport",
    "command undefined"
};


void __START__();
void __MESSAGE__();
void __HELP__();
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

class NEDB{
    private:
    string __SrcDir__;
    Table **__Tables__;
    int __Cursor__;
    int __PageSize__;
    string __Msg__;
    string __Data__;
    void database_init();
public:
    Table *getTable(int);
    Table *getTable(string name);
    void addTable(Table *);
    void dropTable(string name);
    void setData(string data);
    void setMsg(string msg);
    int getCursor();
    ///////////////////////////////////////////////
    string getDir();    //获取当前目录
    int getDefaultPageSize();
    int setDefaultPageSize(int);//设置数据表单页最大字节数
    int setDir(string dir); //设置目录
    string getMsg();    //获取sql语句执行返回信息
    string getData();   //获取sql语句执行数据信息
    NEDB(string dir);   //构造函数
    int dirInit();          //目录初始化
    int scan();             //打开目录下全表
    int open(string fileName);  //载入数据表
    int exec(string sql);   //执行sql语句
    int close();            //关闭数据库
};


#endif