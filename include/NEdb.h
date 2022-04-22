#ifndef __NEDB_H__
#define __NEDB_H__

class NEDB;

typedef struct NEdb{

/////basic///////////////////////////////////////////////

    /**
     * @brief nedb class pointer
     */ 
    NEDB* nedb;

    /**
     * @brief   construct NEdb 
     * @param   full-path of resource folder
     */ 
    NEdb(const char* dir);          //构造函数

/////main-function///////////////////////////////////////

    /**
     * @brief   set full path for NEdb resource folder 
     * @param   full-path of resource folder
     * @return  1:complete, 0:error, -1:pointer error
     */ 
    int setDir(const char* dir);    //设置目录


    /**
     * @brief   scan specified table at NEdb resource folder 
     * @return  1:complete, 0:error, -1:pointer error
     */ 
    int openall();                     //打开目录下全表


    /**
     * @brief   open all tables at NEdb resource folder 
     * @param   table-name
     * @return  1:complete, 0:error, -1:pointer error
     */ 
    int open(const char* fileName); //载入数据表


    /**
     * @brief   execute sql command through NEdb 
     * @param   sql-command
     * @return  1:complete, 0:error, -1:pointer error
     */ 
    int exec(const char* sql);      //执行sql语句


    /**
     * @brief   close and free NEdb 
     * @return  1:complete, 0:error
     */ 
    int close();                    //关闭数据库


    /**
     * @brief   get sql command state, complete or error-message 
     * @return  sql-command state message
     */
    const std::string getMsg();                 //获取sql语句执行返回信息


    /**
     * @brief   get sql command return value, 'select' and 'describe' effective
     * @return  sql-command return value
     */
    const std::string getData();                //获取sql语句执行数据信息


    /**
     * @brief get operate num of select or delete
     * @return sql-operation num                
     */
    int getCount();                             //获取sql查询/删除数据个数信息


/////other-function//////////////////////////////////////

    /**
     * @brief   get NEdb resource folder full path 
     * @return  NEdb resource folder full path
     */
    const std::string getDir();                 //获取当前目录


    /**
     * @brief   get NEdb default page size  
     * @return  NEdb default page-size, default by 0.4KB, -1:pointer error 
     */
    int getDefaultPageSize();


    /**
     * @brief   set NEdb default page size  
     * @param  NEdb default page-size, range: 100-4000 (Bytes)
     * @return  1:complete, 0:error, -1:pointer error
     */
    int setDefaultPageSize(int);    //设置数据表单页最大字节数
    

    /**
     * @brief   initialize NEdb resource folder full path, attempt to create directory automatically
     * @return  1:complete, 0:error, -1:pointer error
     */
    int dirInit();                  //目录初始化
    

}NEdb;

#endif