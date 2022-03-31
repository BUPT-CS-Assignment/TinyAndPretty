#ifndef __NEDB_H__
#define __NEDB_H__


//存储路径
#define __HomeDir__ "/home/jianxf/.nesrc/tables/"

//单数据库最大表数
#define MAX_TABLES 50

//数据库结构体
typedef struct nedb nedb;

/**
 * @brief   自动读取目录下的所有表文件
 * @param   **db 数据库指针地址
 * @param   **msg  操作返回信息
 * @return  1: 操作成功, 0:操作失败
 */
int nedb_auto(nedb** db,char** msg);

/**
 * @brief   读取指定表文件
 * @param   **db 数据库指针地址
 * @param   *table_name 表名称
 * @param   **msg  操作返回信息
 * @return  1: 操作成功, 0:操作失败
 */
int nedb_open(nedb**,const char* table_name,char** msg);

/**
 * @brief   通过sql语句操作数据库
 * @param   **db 数据库指针地址
 * @param   *sql sql语句
 * @param   **data  数据返回信息
 * @param   **msg  操作返回信息
 * @return  1: 操作成功, 0:操作失败
 */
int nedb_exec(nedb*,const char* sql,char** data,char** msg);

/**
 * @brief   关闭数据库
 * @param   **db 数据库指针地址
 * @param   **msg  操作返回信息
 * @return  1: 操作成功, 0:操作失败
 */
int nedb_close(nedb*, char **msg);



#endif