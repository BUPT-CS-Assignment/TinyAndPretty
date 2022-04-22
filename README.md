# NEDB - NOT ENOUGH DATABASE 
### An Ultra Light DataBase Project
![avatar](https://badgen.net/badge/Language/C++11/orange)
![stars](https://badgen.net/badge/Dev%20Env./Linux/green)

## Introduction

**NEDB 是基于 `C++` 的超轻量数据库. 项目参考 `SQLite` 底层原理与 `InnoDB` 引擎, 实现数据库[增-删-查-改]的基本操作, 并提供控制台界面与外部接口.**

## Schedule

### 项目进度 **`2022.4.22`**
- [x] 基本表结构
- [x] 终端控制
- [x] 命令解析器
- [x] 命令执行器
- [x] B+Tree
- [x] 数据表创建
- [x] 数据插入
- [x] 数据查找
- [x] 数据修改
- [x] 数据删除
- [ ] ~~非主键索引~~
- [x] 文件存取
- [x] 异常
- [x] 外部接口
- [ ] 事务锁与并发

---
## Instruction

- ### Basic
  作为超轻量数据库, **NEDB** 支持以下五种数据类型
    |名称|对应 `C++` 数据类型|占用空间|默认值|
    |:---:|:---:|:---:|:---:|
    |int |**`int`**|4|0|
    |int64|**`long long`**|8|0|
    |real|**`double`**|8|0.0|
    |text|**`char[32]`**|32|NULL|
    |longtext|**`char[255]`**|255|NULL|

- ### Console
  ##### **NEDB** 包含以 **`.`** 开头的元命令 AND 以 **`;`** 结尾的普通操作命令
  **NEDB** 支持的元命令如下, 注意严格小写
    |命令|参数|效果|
    |:---:|:---:|:---:|
    |.help|-|显示命令帮助信息|
    |.dir|-|显示当前数据加载目录|
    |.setdir|文件夹路径(-d:default)|设置数据加载目录|
    |.dirinit|-|自动检查及创建当前加载目录|
    |.open|表名称|从数据加载目录加载相应数据表至内存|
    |.openall|-|从数据加载目录加载所有数据表|
    |.size|-|显示当前单数据页最大字节数|
    |.setsize|数据页字节数(-d:default)|设置单数据页最大字节数|
    |.exit|-|退出 **`NEDB`** 控制台|

  **NEDB** 支持的操作命令如下, 注意严格小写
  - ##### 创建数据表
    ```
    create table 'table_name' (
        'Parm_Name' 'Parm_Type',
        ...
        );
    ```
    注意事项 : 
    1. **NEDB** 默认输入的首个元素为主键, 可以在相应元素类型后添加 `key`关键字以指定主键.
    2. **NEDB** 不支持将 `longtext` 类型的数据设置为主键.
    3. 创建指令将搜索当前目录下同一名称的数据表, 无论其是否加载入数据库内存, 若同名数据表存在则创建失败.
    
    操作示例 : 
    ```
    /*
     * 创建名为 'new_table' 的数据表
     * 包含'ID', 'NAME', 'SCORE' 三个字段
     * 指定 'ID' 字段为主键
     */

    create table new_table (
        ID int key, 
        NAME text,
        SCORE real
        );

    ```

  - ##### 插入数据
    ```
    insert into 'table_name' ('parm_name_1', ...) values ('value_1', ...);
    ```
    注意事项 : 
    1. **NEDB** 要求插入数据必须指定主键, 因此在前一参数表中需包含主键字段.
    2. 若省略指定'parm_name'字段, 即
        ```
        insert into 'table_name' values ('value_1', ...);
        ```
        此时参数个数应与表格字段个数相同, **NEDB** 会按照表格字段顺序进行填充.
    3. 当指定字段不存在, 或主键值重复时, **NEDB** 会返回错误信息.
    
    操作示例 : 
    ```
    /*
     * 在 'new_table' 数据表中插入数据行
     * 'ID' 为 2020212001 , 'SCORE' 为 100.0
     */

    insert into new_table (ID, SCORE) values (2020212001, 100.0);

    ```
  - ##### 删除数据
    ```
    delete from 'table_name' where 'condition_1' and ...;
    ```
    注意事项 : 
    1. 当 **where** 语句省略时, 操作会清空全表, 但不删除表.
    2. 注意在 **`and`** 前后需要加上空格.
    
    操作示例 : 
    ```
    /*
     * 在 'new_table' 数据表中删除'NAME'字段为'NULL'的数据行
     */

    delete from new_table where NAME = NULL;

    ```
  - ##### 查询数据
    ```
    select 'parm_name_1', ... from 'table_name' where 'condition_1' and ...;
    ```
    注意事项 : 
    1. 要取出数据行的所有字段, 请使用 **\*** 代替指定字段名称.
    2. 当 **where** 语句省略时, **NEDB** 会遍历全表进行搜索.
    3. 注意在 **`,`** 和 **`and`** 前后需要加上空格.
   
    操作示例 : 
    ```
    /*
     * 在 'new_table' 数据表中查找'SCORE'字段为'90.0'的数据行的所有信息
     */

    select * from new_table where SCORE = 90.0;
    
    ```
  - ##### 修改数据
    ```
    update table_name set 'parm_name_1' = 'value_1', ... where 'condition_1' and ...;
    ```
    注意事项 : 
    1. 主键值不能被修改.
    2. 当 **where** 语句省略时, **NEDB** 会将全表数据进行相应替换.
    3. 注意在 **`and`** 前后需要加上空格.
   
    操作示例 :
    ```
    /*
     * 在 'new_table' 数据表中将'ID'字段为'2020212001'的数据行的'SCORE'信息修改为'150.0'
     */

    update new_table set SCORE = 150.0 where ID = 2020212001;
    
    ```

  - ##### 获取数据表结构
    ```
    describe table 'table_name';

    ```
    注意事项 : 
    1. 此操作将返回该数据表单页最大字节数, 字段名称和字段类型.

  - ##### 删除数据表
    ```
    drop table 'table_name';

    ```
    注意事项 : 
    1. 此操作将直接删除数据文件, 请谨慎操作.

  - ##### 列出所有数据表
    ```
    select tables;

    ```
    注意事项 : 
    1. 此操作将返回加载到内存中的所有表名称.

- ### Interface
  - ##### 简介
    **`NEDB`** 提供简易的对外接口, 以整合入各类项目使用. 接口格式参考 **`SQLite`** 的主要接口格式, 包含数据库创建, 文件读取, 指令执行三大核心功能.
  
  - ##### 使用说明
    1. **`NEDB`** 外部接口静态库文件为 **`libnedb.a`** , 动态库为 **`libnedb.so`** , 入口头文件为 **`NEdb.h`** , 只需在其他项目中引用该头文件并再编译时连接该静态库/动态库即可. 
        ```
        /**
        * @ g++ 参考指令
        *  
        * -o out 指定输出文件名
        * -L ./  指定库路径
        * -l ne  链接静态库, nedb为'libnedb.a'去掉前缀'lib'和后缀'.so'
        */
        g++ main.cpp -o out -L ./ -l ne

        ```
    2. 默认单数据库结构体指针能存 **`200`** 张数据表.
    3. 为便于测试, 默认单页数据最大存储量为 **`0.4KB`** , 可通过接口修改.
    4. 接口当前仅限 **`C++`** 语言.

  - ##### 接口说明
    1. 数据库结构体  

        ```
        /**
        * @brief NEDB class pointer
        */ 
        NEDB* nedb;

        ```
        i. 类名称定义为 **`NEdb`** , 后续所有操作都基于结构体.  
        ii. 结构体初始化时强制要求设定数据文件加载目录, 请确保路径正确且权限足够.
        iii. 接口的调用以 **`结构体名.函数名`** 的方式实现
        

    2. 主要接口

        ```
        /**
         * @brief   construct NEdb 
         * @param   full-path of resource folder
         */ 
        NEdb(const char* dir);          //构造函数

        ```
        ```
        /**
         * @brief   set full path for NEdb resource folder 
         * @param   full-path of resource folder
         * @return  1:complete, 0:error, -1:pointer error
         */ 
        int setDir(const char* dir);    //设置目录

        ```
        ```
        /**
         * @brief   scan specified table at NEdb resource folder 
         * @return  1:complete, 0:error, -1:pointer error
         */ 
        int scan();                     //打开目录下全表

        ```
        ```
        /**
         * @brief   scan all tables at NEdb resource folder 
         * @param   table-name
         * @return  1:complete, 0:error, -1:pointer error
         */ 
        int open(const char* fileName); //载入数据表

        ```
        ```

        /**
         * @brief   execute sql command through NEdb 
         * @param   sql-command
         * @return  1:complete, 0:error, -1:pointer error
         */ 
        int exec(const char* sql);      //执行sql语句

        ```
        ```
        /**
         * @brief   close and free NEdb 
         * @return  1:complete, 0:error
         */ 
        int close();                    //关闭数据库
        
        ```
        ```
        /**
         * @brief   get sql command state, complete or error-message 
         * @return  sql-command state message
         */
        char* getMsg();                 //获取sql语句执行返回信息
        
        ```
        ```
        /**
         * @brief   get sql command return value, 'select' and 'describe' effective
         * @return  sql-command return value
         */
        char* getData();                //获取sql语句执行数据信息

        ```
        ```
        /**
         * @brief get operate num of select or delete
         * @return sql-operation num                
         */
        int getCount();                 //获取sql查询/删除数据个数信息
        ```

    3. 其他功能接口
        ```
        /**
         * @brief   get NEdb resource folder full path 
         * @return  NEdb resource folder full path
         */
        char* getDir();                 //获取当前目录

        ```
        ```
        /**
         * @brief   get NEdb default page size  
         * @return  NEdb default page-size, default by 0.4KB, -1:pointer error 
         */
        int getDefaultPageSize();

        ```
        ```
        /**
         * @brief   set NEdb default page size  
         * @param  NEdb default page-size, range: 100-4000 (Bytes)
         * @return  1:complete, 0:error, -1:pointer error
         */
        int setDefaultPageSize(int);    //设置数据表单页最大字节数

        ```
        ```
        /**
         * @brief   initialize NEdb resource folder full path, attempt to create directory automatically
         * @return  1:complete, 0:error, -1:pointer error
         */
        int dirInit();                  //目录初始化

        ```

## Structure
- ### Principle
  - ##### 文件格式
    1. 在终端, **NEDB** 文件存储默认位置为 **`/home/{user_name}/.nesrc/`** , 可通过 **`.setdir`** 命令进行修改.
    2. 调用外部接口时默认构造函数需指定数据文件夹位置, 可通过 **`setDir('dir')`** 函数进行更改.
    3. 一张数据表将包含以下三个文件
        |文件名|内容|
        |:---:|:---:|
        |table_name.nef|表结构文件, 包含表名和字段信息等|
        |table_name.ned|数据文件, 包含写入表格的所有数据|
        |~~table_name.nei~~|~~索引文件, 包含用户创建的非主键索引~~|

  - ##### 存储原理
    1. **NEDB** 底层以 **`B+Tree`** 结构进行数据处理.
    2. 数据存储以行为最小单位, 页为基本单位, 页大小默认为 **`0.4KB`**, 可在控制台中或通过接口进行修改. 单页可存最大行数由表结构决定.
    3. **`B+Tree`** 叶子节点存有数据页起始位置在数据文件中的偏移量, 一次 **`I/O`** 操作会将整页数据读入内存, 操作完毕后重新写入.
  - ##### 查询原理
    **(working)**

- ### Modules
    **NEDB** 项目包含以下结构模块
    |模块|内容|
    |:---:|:---:|
    |Nedb|基本运行和控制|
    |Process|输入输出流, 命令解析与执行|
    |Data|数据表, 数据页, 数据行基础操作|
    |BalanceTree|底层索引B+树模板|
    |Operation|增, 删, 查, 改基本操作|
    |Storage|文件读写存储|
    |NEexception|异常处理|
    |Implement|排序, 判溢出等小工具集|


- ### Directory
    





