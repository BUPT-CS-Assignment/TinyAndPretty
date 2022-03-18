#ifndef __TABLE_H__
#define __TABLE_H__
#include<Basic.h>

class Table{
class Page;
class Row;
private:
    int __Table_ID; //表ID索引
    int __Cursor;   //页光标
    string __Name;  //表名
    Page** __Pages; //页指针数组
    /**
     *数据表格式相关
     */
    string* __Data_ID;    //数据元素名称数组
    int __Data_Num; //数据元素数量
    __DataType__*  __Data_Type;   //数据类型数组
public:
    Table(int id, string name); //构造函数
    bool init(string statement);    //初始化表数据类型
    /**
     *数据操作相关
     */
    bool insert(string statement);  //数据行插入
    void add_page(Page* Node);   //添加页
    bool delete_page(int page_number);  //删除页
    bool delete_row(int page_number,int row_number);    //删除指定行
    bool remove_table();    //删除表
    void print_table(); //打印表
    //
    __DataType__* getDataType(); //获取数据元素类型
    string getName();   //获取表名
    Page* getPage(int page_number);
};

#endif