#ifndef __NEDB_H__
#define __NEDB_H__
#include <iostream>

void NEDB_SETTING(
    int MAX_TABLES = 64,          /* Max Table Numbers */ 
    int DEFAULT_PAGE_SIZE = 400,  /* Default Page Size */
    int SIG_WAIT_MSECS = 100,     /* Time(ms) Wait For Unlock */
    int SIG_CHECK_TIMES = 5       /* Check Times While Waiting */
);

void NEDB_DEBUG(int SIG_DEBUG = 0 /* Debug Signal */);


/**
 * @brief ErrorCode
 *
 *
 * [ Task Complete ]
 *  0    NO_ERROR
 *
 * [ System Error ]
 * -1    NULL_POINTER
 *  1    SYSTEM_ERROR
 *  2    SIZE_NOT_ALLOWED
 *
 * [ Dir Secion ]
 *  3    DIR_ERROR
 *
 * [ File Section ]
 *  4    FILE_OPEN_ERROR
 *  5    FILE_NOT_FOUND
 *  6    FILE_DAMAGED
 *
 * [ Sql Section ]
 *  7    SQL_FORM_ERROR
 *  8    SQL_UNDEFINED
 *
 * [ Task Lock Section ]
 *  9    ACTION_BUSY
 *
 * [ Table Section ]
 *  10   TABLE_EXIST
 *  11   TABLE_NOT_FOUND
 *  12   TABLE_NUM_REACH_LIMIT
 *
 * [ Param Section ]
 *  13   PARAM_EMPTY
 *  14   PARAM_FORM_ERROR
 *  15   PARAM_NOT_FOUND
 *  16   PARAM_NUM_MISMATCH
 *  17   VALUE_EMPTY
 *
 * [ Data Type Section ]
 *  18   TYPE_UNDEFINED
 *  19   TYPE_INT_MISMATCH
 *  20   TYPE_INT64_MISMATCH
 *  21   TYPE_REAL_MISMATCH
 *  22   TYPE_INT_OVERFLOW
 *  23   TYPE_INT64_OVERFLOW
 *  24   TYPE_REAL_OVERFLOW
 *  25   TYPE_TEXT_OVERFLOW
 *  26   TYPE_LONGTEXT_OVERFLOW
 *
 * [ Primary Key Section ]
 *  27   KEY_TYPE_NOT_ALLOWED
 *  28   KEY_VAL_EXIST
 *  29   KEY_VAL_REQUIRED
 *  30   KEY_VAL_CHANGE_NOT_ALLOWED
 *
 * [ Data Error ]
 *  31   DATA_NOT_FOUND
 *
 */

class DataBase;
typedef struct NEDB{

    /////basic///////////////////////////////////////////////

        /**
         * @brief nedb class pointer
         */
    DataBase* nedb;

    /**
     * @brief   construct NEDB
     * @param   full-path of resource folder
     */
    NEDB(const char* dir);          //构造函数

/////main-function///////////////////////////////////////

    /**
     * @brief   set full path for NEDB resource folder
     * @param   full-path of resource folder
     * @return  1:complete, 0:error, -1:pointer error
     */
    int SetDir(const char* dir);    //设置目录


    /**
     * @brief   scan specified table at NEDB resource folder
     * @return  1:complete, 0:error, -1:pointer error
     */
    int Openall();                     //打开目录下全表


    /**
     * @brief   open all tables at NEDB resource folder
     * @param   table-name
     * @return  1:complete, 0:error, -1:pointer error
     */
    int Open(const char* fileName); //载入数据表


    /**
     * @brief   execute sql command through NEDB
     * @param   sql-command
     * @return  error code
     */
    int Exec(const char* sql);      //执行sql语句


    /**
     * @brief   close and free NEDB
     * @return  1:complete, 0:error
     */
    int Close();                    //关闭数据库


    /**
     * @brief   get sql command state, complete or error-message
     * @return  sql-command state message
     */
    int ErrCode();                 //获取sql语句执行返回信息


    /**
     * @brief   get sql command return value, 'select' and 'describe' effective
     * @return  sql-command return value
     */
    const std::string ReturnVal();                //获取sql语句执行数据信息


    /**
     * @brief get operate num of select or delete
     * @return sql-operation num
     */
    int Count();                             //获取sql查询/删除数据个数信息


/////other-function//////////////////////////////////////

    /**
     * @brief   get NEDB resource folder full path
     * @return  NEDB resource folder full path
     */
    const std::string GetDir();                 //获取当前目录


    /**
     * @brief   get NEDB default page size
     * @return  NEDB default page-size, default by 0.4KB, -1:pointer error
     */
    int DefaultPageSize();


    /**
     * @brief   set NEDB default page size
     * @param  NEDB default page-size, range: 100-4000 (Bytes)
     * @return  1:complete, 0:error, -1:pointer error
     */
    int SetDefaultPageSize(int);    //设置数据表单页最大字节数


    /**
     * @brief   initialize NEDB resource folder full path, attempt to create directory automatically
     * @return  1:complete, 0:error, -1:pointer error
     */
    int DirInit();                  //目录初始化


}NEDB;

#endif