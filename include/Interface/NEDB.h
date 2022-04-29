#ifndef __NEDB_H__
#define __NEDB_H__
#include <iostream>

namespace NEDBSTD{


    /* <------------------------- Basic NEDB infomation --------------------------> */

    /* NEDB debug sign */
    #define DEBUG_NONE          0
    #define DEBUG_SIMPLE        1
    #define DEBUG_DETAIL        2  

    /**
     * @brief change NEDB default settings
     *
     * @param MAX_TABLES Max Table Numbers
     * @param DEFAULT_PAGE_SIZE Default Page Size
     * @param SIG_WAIT_MSECS Time(ms) Wait For Unlock
     * @param SIG_CHECK_TIMES Check Times While Waiting
     */
    void NEDB_SETTING(int MAX_TABLES = 64, int DEFAULT_PAGE_SIZE = 400, int SIG_WAIT_MSECS = 100, int SIG_CHECK_TIMES = 5);

    /**
     * @brief set NEDB debug signal
     *
     * @param SIG_DEBUG debug signal
     */
    void NEDB_DEBUG(int SIG_DEBUG = 0);

    /**
     * @brief set NEDB time-show flag
     * 
     * @param flag time-show signal
     */
    void NEDB_TIME_FLAG(bool FLAG = 1);

    /**
     * @brief ErrorCode (NEDB Exception)
     *
     */
    enum NEexception:__uint16_t{
        /* [ NoError ] */
        /*-- 0 --*/  NO_ERROR,

        /* [ System Error ] */
        /*-- 1 --*/  SYSTEM_ERROR,
        /*-- 2 --*/  SIZE_NOT_ALLOWED,

        /* [ Dir Secion ] */
        /*-- 3 --*/  DIR_ERROR,

        /* [ File Section ] */
        /*-- 4 --*/  FILE_OPEN_ERROR,
        /*-- 5 --*/  FILE_NOT_FOUND,
        /*-- 6 --*/  FILE_DAMAGED,

        /* [ Sql Section ] */
        /*-- 7 --*/  SQL_FORM_ERROR,
        /*-- 8 --*/  SQL_UNDEFINED,

        /* [ Task Lock Section ] */
        /*-- 9 --*/  ACTION_BUSY,

        /* [ Table Section ] */
        /*-- 10--*/  TABLE_EXIST,
        /*-- 11--*/  TABLE_NOT_FOUND,
        /*-- 12--*/  TABLE_NUM_REACH_LIMIT,

        /* [ Param Section ] */
        /*-- 13--*/  PARAM_EMPTY,
        /*-- 14--*/  PARAM_FORM_ERROR,
        /*-- 15--*/  PARAM_NOT_FOUND,
        /*-- 16--*/  PARAM_NUM_MISMATCH,
        /*-- 17--*/  VALUE_EMPTY,

        /* [ Data Type Section ] */
        /*-- 18--*/  TYPE_UNDEFINED,
        /*-- 19--*/  TYPE_INT_MISMATCH,
        /*-- 20--*/  TYPE_INT64_MISMATCH,
        /*-- 21--*/  TYPE_REAL_MISMATCH,
        /*-- 22--*/  TYPE_INT_OVERFLOW,
        /*-- 23--*/  TYPE_INT64_OVERFLOW,
        /*-- 24--*/  TYPE_REAL_OVERFLOW,
        /*-- 25--*/  TYPE_TEXT_OVERFLOW,
        /*-- 26--*/  TYPE_LONGTEXT_OVERFLOW,

        /* [ Primary Key Section ] */
        /*-- 27--*/  KEY_TYPE_NOT_ALLOWED,
        /*-- 28--*/  KEY_VAL_EXIST,
        /*-- 29--*/  KEY_VAL_REQUIRED,
        /*-- 30--*/  KEY_VAL_CHANGE_NOT_ALLOWED,

        /* [ Data Section ] */
        /*-- 31--*/  DATA_NOT_FOUND,
        /*-- 32--*/  COMPARE_NONSUPPORT,

        /* [Console Section ] */
        /*-- 33--*/  COMMAND_UNDEFINED
    };


    /* <----------------------------- NEDB Class -------------------------------> */

    /**
     * @brief basic database class
     * 
     */
    class DataBase;

    /**
     * @brief NEDB class interface
     *
     */
    class NEDB{
    private:
        /**
         * @brief DataBase class pointer
         *
         */
        DataBase* nedb;
    public:


    /* <--------------------------- Basic Function --------------------------> */
        
        /**
         * @brief Construct a new NEDB object
         *
         * @param dir default dir
         */
        NEDB(const std::string dir);          //构造函数

        /**
         * @brief   close and free NEDB
         * @return  error code
         */
        int Close();    //关闭数据库


    /* <--------------------------- File Function ---------------------------> */

        /**
         * @brief mount table with full path without suffix
         *
         * @param filePath full table-path without suffix
         * @return int ErrorCode
         */
        int Open(const std::string filePath); //载入数据表

        /**
         * @brief load table with table name at current dir
         *
         * @param fileName table-name
         * @return int ErrorCode
         */
        int Mount(const std::string fileName); //挂载数据表

        /**
         * @brief load all tables at current dir
         *
         * @param count mount numbers
         * @return int ErrorCode
         */
        int MountAll(int& count);                     //打开目录下全表

        /**
         * @brief unmout table from RAM
         *
         * @param tableName
         * @return int ErrorCode
         */
        int Unmount(const std::string tableName); //取消挂载数据表

        /**
         * @brief delte table from RAM and ROM
         *
         * @param table name
         * @return int ErrorCode
         */
        int Drop(const std::string table);  //删除数据表


    /* <--------------------------- Query Function --------------------------> */

        /**
         * @brief execute sql command through NEDB
         *
         * @param sql sql-command
         * @param cout operated number
         * @param retVal operation return value
         * @return int ErrorCode
         */
        int Query(const std::string sql, int& cout, std::string& retVal);      //执行sql语句

        /**
         * @brief create table
         *
         * @param name table name
         * @param setField filed to set
         * @return int ErrorCode
         */
        int Create(const std::string name, const std::string setField);

        /**
         * @brief insert values into specified table
         *
         * @param table table name
         * @param fields field of insert values
         * @param values insert values divided by ','
         * @return int ErrorCode
         */
        int Insert(const std::string table, const std::string fields, const std::string values);

        /**
         * @brief delete values from specified table
         *
         * @param table table name
         * @param conditions delete conditions divided by ' and '
         * @param count number of deleted value
         * @return int ErrorCode
         */
        int Delete(const std::string table, const std::string conditions, int& count);

        /**
         * @brief select values from specified table
         *
         * @param table table name
         * @param fields field of select values divided by ' , '
         * @param conditions select conditions divided by ' and '
         * @param count number of selected value
         * @param retVal selected result
         * @return int ErrorCode
         */
        int Select(const std::string table, const std::string fields, const std::string conditions, int& count, std::string& retVal);

        /**
         * @brief update values from specified table
         *
         * @param table table name
         * @param setVal value to set divided by ' , '
         * @param conditions update conditions divided by ' and '
         * @param count number of updated value
         * @return int ErrorCode
         */
        int Update(const std::string table, const std::string setVal, const std::string conditions, int& count);

        /**
         * @brief describe basic table infomation
         * 
         * @param table table name
         * @param retVal return value
         * @return int ErrorCode
         */
        int Describe(const std::string table, std::string& retVal);


    /* <------------------------- Directory Function --------------------------> */

        /**
         * @brief initialize NEDB resource folder full path, attempt to create directory automatically
         *
         * @return int ErrorCode
         */
        int DirInit();                  //目录初始化

        /**
         * @brief set full path for NEDB resource folder
         * @param dir full-path of resource folder
         * @return int ErrorCode
         */
        int SetDir(const std::string dir);    //设置目录

        /**
         * @brief Get NEDB current dir
         *
         * @return const std::string Dir
         */
        const std::string GetDir();                 //获取当前目录

        /**
         * @brief set NEDB default page size
         * @param size NEDB default page-size, range: 100-4000 (Bytes)
         * @return ErrorCode
         */
        int SetDefaultPageSize(int size);    //设置数据表单页最大字节数

        /**
         * @brief   get NEDB default page size
         *
         * @return  int NEDB default page-size, default by 0.4KB, -1:pointer error
         */
        int DefaultPageSize();          //获取数据表单页最大字节数

    };

    /**
     * @brief NEDB Exception Name
     *
     */
    static std::string const NEexceptionName[] = {
        "NO_ERROR",
        "SYSTEM_ERROR",
        "SIZE_NOT_ALLOWED",
        "DIR_ERROR",
        "FILE_OPEN_ERROR",
        "FILE_NOT_FOUND",
        "FILE_DAMAGED",
        "SQL_FORM_ERROR",
        "SQL_UNDEFINED",
        "ACTION_BUSY",
        "TABLE_EXIST",
        "TABLE_NOT_FOUND",
        "TABLE_NUM_REACH_LIMIT",
        "PARAM_EMPTY",
        "PARAM_FORM_ERROR",
        "PARAM_NOT_FOUND",
        "PARAM_NUM_MISMATCH",
        "VALUE_EMPTY",
        "TYPE_UNDEFINED",
        "TYPE_INT_MISMATCH",
        "TYPE_INT64_MISMATCH",
        "TYPE_REAL_MISMATCH",
        "TYPE_INT_OVERFLOW",
        "TYPE_INT64_OVERFLOW",
        "TYPE_REAL_OVERFLOW",
        "TYPE_TEXT_OVERFLOW",
        "TYPE_LONGTEXT_OVERFLOW",
        "KEY_TYPE_NOT_ALLOWED",
        "KEY_VAL_EXIST",
        "KEY_VAL_REQUIRED",
        "KEY_VAL_CHANGE_NOT_ALLOWED",
        "DATA_NOT_FOUND",
        "COMPARE_NONSUPPORT",
        "COMMAND_UNDEFINED"
    };

}

#endif