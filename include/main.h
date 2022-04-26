#ifndef __MAIN_H__
#define __MAIN_H__

#include <iostream>
#include <string.h>
#include <cstdlib>
#include <cstdio>
#include <stdarg.h>
#include <stdio.h>
#include <regex>
#include <iomanip>
#include <unistd.h>
#include <sys/io.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

/* Basic Sign */
#define TEXT_LENGTH         32
#define LONGTEXT_LENGTH     255
#define PAGE_HEAD_SIZE      40 
#define DATA_OFFSET         2

/* Lock Status */
#define SIG_BLOCK           0
#define SIG_RUN             1
#define SIG_FREE            2



/* DeBug Level */
#define DEBUG_NONE          0
#define DEBUG_SIMPLE        1
#define DEBUG_DETAIL        2

extern int SIG_DEBUG;
extern int SIG_WAIT_MSECS;
extern int SIG_CHECK_TIMES;
extern int MAX_TABLES;
extern int DEFAULT_PAGE_SIZE;

/* File Format */
#define __FramSuffix__      ".nef"
#define __DataSuffix__      ".ned"
#define __IndexSuffix__     ".nei"
#define __DefaultDir__      "/home/jianxf/.nesrc/"

/* NEDB Data Type Support */
enum DATA_TYPE: __uint16_t{
    __INT,
    __INT64,
    __REAL,
    __TEXT,
    __LONGTEXT
};
static std::string const kTypeName[] = {"int","int64","real","text","longtext"};
static int const kTypeSize[] = {4,8,8,32,255};


/* Command Sign */
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
    __SHOWPAGESIZE,
    __DEBUGSET
};

/* Operation Sign */
enum OPERATION: __uint16_t{
    CREATE_TABLE,
    INSERT_VALUES,
    DELETE_VALUES,
    UPDATE_VALUES,
    SELECT_VALUES,
    SELECT_TABLES,
    DESCRIBE_TABLE,
    DROP_TABLE,
    UNDEFINED
};

/* Exception Sign */
enum NEexception:__uint16_t{
    //NoError
    NO_ERROR,
    //system
    SYSTEM_ERROR, SIZE_NOT_ALLOWED,
    //dir error
    DIR_ERROR,
    //file error
    FILE_OPEN_ERROR, FILE_NOT_FOUND, FILE_DAMAGED,
    //sql error
    SQL_FORM_ERROR, SQL_UNDEFINED,
    //lock status
    ACTION_BUSY,
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


////////////////////////////////////////////////////////////////
static std::string const NEexceptionName[] = {
    //no error
    "no error",
    //system
    "system error","size not allowed",
    //dir error
    "dir error",
    //file error
    "file open error", "file not found", "file damaged",
    //sql error
    "sql form mismatch", "sql undefined",
    //lock status
    "action busy",
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

union INDEX{
    int i_index;
    long long l_index;
    double d_index;
    char t_index[32];
};

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

/* DataBase Class */
class DataBase{
    private:
    std::string     __SrcDir__;
    Table**         __Tables__;
    int             __Cursor__;
    int             __PageSize__;
    int             __ErrCode__;
    std::string     __ReturnVal__;
    int             __OperateCount__;

    ///////////////////////////////////////////////
    void database_init();
    public:
    DataBase(std::string dir);              //构造函数

    /* DataBase Lock */
    int     __ProcStatus__;         //DataBase Lock
    int&    Status();
    void    SetStatus(int state = SIG_BLOCK);

    /* Dir Operation */
    std::string getDir();                   //获取当前目录
    int     setDir(std::string dir);        //设置目录 
    int     dirInit();                      //目录初始化

    /* PageSize Operation */
    int     getDefaultPageSize();
    int     setDefaultPageSize(int);        //设置数据表单页最大字节数

    /* Return Section */
    int     getErrCode();                   //获取返回代码
    std::string getReturnVal();             //获取返回值
    int     getCount();                     //获取执行行数

    /* File Section */
    int     openall();                      //打开目录下全表
    int     open(std::string fileName);     //载入数据表

    /* Execute Section */
    int     exec(std::string sql);          //执行sql语句
    void    setReturnVal(std::string);      //设置返回值
    void    setErrCode(int);                //设置返回代码
    void    setCount(int);                  //设置执行行数
    void    AddCount();                     //执行行数加1
    void    SubCount();                     //执行函数减1

    /* Basic Section */
    int     close();                        //关闭数据库
    Table*  getTable(int);
    Table*  getTable(std::string name);
    void    addTable(Table*);
    void    dropTable(std::string name);
    int     getCursor();

};


/* Console Function */
void __START__();
void __MESSAGE__();
void __HELP__();
int __DEBUG_SET__(std::string);

#endif