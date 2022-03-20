#ifndef __DATA_H__
#define __DATA_H__
#include<Basic.h>
#include<BalanceTree.h>


class Table{
class Page;
class Row;
private:
    int __Table_ID; //表ID索引
    int __Page_Num;   //页总数
    int __Page_ID;  //页标识[不重复]
    string __Name;  //表名
    BalanceTree<Page,string>* __Pages; //树结构数组
    //Page** __Pages; //页指针数组
    /**
     *数据表格式相关
     */
    string* __Data_ID;    //数据元素名称数组
    int __Data_Num; //数据元素数量
    __DataType__*  __Data_Type;   //数据类型数组
    __DataType__ __Index_Type;  //索引类型
    string __Index_ID;  //索引ID
public:
    Table(int id, string name); //构造函数
    bool init(string statement);    //初始化表数据类型
    /**
     *数据操作相关
     */
    bool insert(string statement);  //数据行插入
    void add_page(Page* Node);   //添加页
    bool delete_page(string index);  //删除页
    bool delete_data(string index);    //删除指定行
    bool remove_table();    //删除表
    void print_table(); //打印表
    //
    __DataType__* getDataType(); //获取数据元素类型
    string getName();   //获取表名
};

class Table::Page{
public:
    int __Page_Num;    //页码索引
    int __Cursor;   //行光标
    Row** __Rows;   //行指针数组
    Table* t;
    string __Index;  //索引指针
public:
    Page(int page_num,Table* t); //构造函数
    bool isFull();  //判断满页
    int getCursor();    //获取光标位置
    void insert(Row* new_row); //插入行
    bool delete_row(string index);    //按索引删除
    bool remove_page(); //删除整页
    void print_page();  //打印整页
    string getIndex();
};

class Table::Row{
/**
 * @brief   [C]数据行 
 * 
 */
private:
    int __Row_Num; //数据行标识
    string __Index;  //索引
    void** __Content; //数据指针数组(未定义类型)
    Table* t;
public:
    Row(Table* t);  //构造函数
    bool padding(string statement); //内容填充
    char* format(); //格式化转化输出
    void erase();   //内容清空
    string getIndex();
    bool operator<(Row&);
    bool operator>(Row&);
    bool operator==(Row&);
    bool operator<=(Row&);
    bool operator>=(Row&);
};





#endif
