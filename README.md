# LiteDB - A Lite DataBase Project

![avatar](https://badgen.net/badge/Language/C++11/orange)
![stars](https://badgen.net/badge/Dev%20Env./Linux/green)

## Introduction

**LiteDB 是基于 `C++` 的超轻量数据库. 项目参考 `SQLite` 底层原理与 `InnoDB` 引擎, 实现数据库[增-删-查-改]的基本操作, 并提供控制台界面与对外接口.**

## Instruction

- ### Basic
  作为超轻量数据库, **LiteDB** 支持以下五种数据类型
    |名称|对应 `C++` 数据类型|占用空间|默认值|
    |:---:|:---:|:---:|:---:|
    |INT |**`int`**|4|0|
    |INT64|**`long long`**|8|0|
    |REAL|**`double`**|8|0.0|
    |TEXT|**`char[32]`**|32|NULL|
    |LONGTEXT|**`char[255]`**|255|NULL|

- ### Console
  ##### **LiteDB** 包含以 **`.`** 开头的元命令 AND 以 **`;`** 结尾的普通操作命令
  **LiteDB** 支持的元命令如下, 注意严格小写
    |命令|参数|效果|
    |:---:|:---:|:---:|
    |.help|-|显示命令帮助信息|
    |.open|表名称|从LiteDB根目录加载相应数据表至内存|
    |.openall|-|从LiteDB根目录加载所有数据表|
    |.exit|-|退出 **`LiteDB`** 控制台|

  **LiteDB** 支持的操作命令如下, 注意严格大写
  - ##### 创建数据表
    ```
    CREATE TABLE 'table_name' (
        'Parm_Name' 'Parm_Type',
        ...
        );
    ```
    注意事项 : 
    1. **LiteDB** 默认输入的首个元素为主键, 可以在相应元素类型后添加 `KEY`关键字以指定主键.
    2. **LiteDB** 不支持将 `LONGTEXT` 类型的数据设置为主键.
    
    操作示例 : 
    ```
    /*
     * 创建名为 'new_table' 的数据表
     * 包含'ID', 'NAME', 'SCORE' 三个字段
     * 指定 'ID' 字段为主键
     */

    CREATE TABLE new_table (
        ID INT KEY, 
        NAME TEXT,
        SCORE REAL
        );

    ```

  - ##### 插入数据
    ```
    INSERT INTO 'table_name' ('Parm_Name_1', ...) VALUES ('Value_1', ...);
    ```
    注意事项 : 
    1. **LiteDB** 要求插入数据必须指定主键, 因此在前一参数表中需包含主键字段.
    2. 若省略指定'Parm_Name'字段, 即
        ```
        INSERT INTO 'table_name' VALUES ('Value_1', ...);
        ```
        此时参数个数应与表格字段个数相同, **LiteDB** 会按照表格字段顺序进行填充.
    3. 当指定字段不存在, 或主键值重复时, **LiteDB** 会返回错误信息.
    
    操作示例 : 
    ```
    /*
     * 在 'new_table' 数据表中插入数据行
     * 'ID' 为 2020212001 , 'SCORE' 为 100.0
     */

    INSERT INTO new_table (ID, SCORE) VALUES (2020212001, 100.0);

    ```
  - ##### 删除数据
    ```
    DELETE FROM 'table_name' WHERE 'condition';
    ```
    注意事项 : 
    1. **LiteDB** 仅支持单一条件, 即不支持 'condition_1 AND condtion_2' 语句.
    2. 当 **WHERE** 语句省略时, 操作会清空全表, 但不删除表
    
    操作示例 : 
    ```
    /*
     * 在 'new_table' 数据表中删除'NAME'字段为'NULL'的数据行
     */

    DELETE FROM new_table WHERE NAME = NULL;

    ```
  - ##### 查询数据
    ```
    SELECT 'parm_name_1', ... FROM 'table_name' WHERE 'condition' 
    ```
    注意事项 : 
    1. 要取出数据行的所有字段, 请使用 **\*** 代替指定字段名称.
    2. 当 **WHERE** 语句省略时, **LiteDB** 会遍历全表进行搜索
    3. **LiteDB** 仅支持单一条件, 即不支持 'condition_1 AND condtion_2' 语句.
   
    操作示例 : 
    ```
    /*
     * 在 'new_table' 数据表中查找'SCORE'字段为'90.0'的数据行的所有信息
     */

    SELECT * FROM new_table WHERE SCORE = 90.0;
    
    ```
  - ##### 修改数据
    ```
    UPDATE table_name SET 'parm_name_1' = 'value_1', ... WHERE 'condition' 
    ```
    注意事项 : 
    1. 主键值不能被修改.
    2. 当 **WHERE** 语句省略时, **LiteDB** 会将全表数据进行相应替换.
    3. **LiteDB** 仅支持单一条件, 即不支持 'condition_1 AND condtion_2' 语句.
   
    操作示例 :
    ```
    /*
     * 在 'new_table' 数据表中将'ID'字段为'2020212001'的数据行的'SCORE'信息修改为'150.0'
     */

    UPDATE new_table SET SCORE = 150.0 WHERE ID = 2020212001;
    
    ```
- ### Interface
    **(working)**

## Structure
- ### Principle
  - ##### 文件格式
    1. **LiteDB** 文件存储默认位置为 **`/home/{user_name}/LiteDB/Tables`** , 可在 **`Basic.h`** 中进行修改.
    2. 一张数据表将包含以下三个文件
        |文件名|内容|
        |:---:|:---:|
        |table_name.ldf|表结构文件, 包含表名和字段信息等|
        |table_name.ldd|数据文件, 包含写入表格的所有数据|
        |table_name.ldi|索引文件, 包含用户创建的非主键索引|

  - ##### 存储原理
    1. **LiteDB** 底层以 **`B+Tree`** 结构进行数据处理.
    2. 数据存储以行为最小单位, 页为基本单位, 页大小默认为 **`4KB`**, 可在 **`Basic.h`** 文件中进行修改. 单页可存最大行数由表结构决定.
    3. **`B+Tree`** 叶子节点存有数据页起始位置在数据文件中的偏移量, 一次 **`I/O`** 操作会将整页数据读入内存, 操作完毕后重新写入.
  - ##### 查询原理
    **(working)**
- ### Modules
    **LiteDB** 项目包含以下结构模块
        |模块|内容|
        |:---:|:---:|
        |Basic|基本运行|
        |Process|输入输出流, 命令解析与执行|
        |Data|数据表, 数据页, 数据行基础操作|
        |BalanceTree|底层索引B+树模板|
        |Operation|增, 删, 查, 改基本操作|
        |Storage|文件读写存储|
        |Exception|异常处理|
        |Implement|排序, 判溢出等小工具集|


- ### Directory
    





