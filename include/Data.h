#ifndef __DATA_H__
#define __DATA_H__
#include<Basic.h>
#include<BalanceTree.h>

class Index{
friend class Table;
    int i_index;
    long long l_index;
    double d_index;
    string s_index;
    __DataType__ index_type;
public:
    Index(string index,__DataType__);
    Index();
    bool operator<(Index&);
    bool operator>(Index&);
    bool operator==(Index&);
    bool operator<=(Index&);
    bool operator>=(Index&);
    friend ostream & operator << (ostream & out, Index & index);
};

class Table{
class Page;
class Row;
friend class Index;
private:
    int __Table_ID; //表ID索引
    int __Page_Num;   //页总数
    int __Page_ID;  //页标识[不重复]
    string __Name;  //表名
    /**
     * 索引多类型
     */
    BalanceTree<Page,Index>* __Pages;
    //Page** __Pages; //页指针数组
    /**
     *数据表格式相关
     */
    string* __Data_ID;    //数据元素名称数组
    int __Data_Num; //数据元素数量
    __DataType__*  __Data_Type;   //数据类型数组
    __DataType__ __Index_Type;  //索引类型
    string __Index_ID;  //索引ID
    Page* page_locate(Index);   //数据页搜索
    //

public:
    Table(int id, string name); //构造函数
    bool init(string statement);    //初始化表数据类型
    /**
     *数据操作相关
     */
    bool insert(string statement);  //数据行插入
    void add_page(Page* Node);   //添加页
    bool delete_page(Index&);  //删除页
    bool delete_row(Index&);    //删除指定行
    bool remove_table();    //删除表
    void print_table(); //打印表
    void print_structure(); //打印表结构
    //
    __DataType__* getDataType(); //获取数据元素类型
    string getName();   //获取表名
    __DataType__ getIndexType();
};

class Table::Page{
friend class Index;
public:
    int __Page_Num;    //页码索引
    int __Cursor;   //行光标
    Row** __Rows;   //行指针数组
    Table* t;
    Index __Index;  //索引指针
public:
    Page(int page_num,Table* t); //构造函数
    bool isFull();  //判断满页
    int getCursor();    //获取光标位置
    bool insert(Row* new_row); //插入行
    bool delete_row(Index& index);    //按索引删除
    bool remove_page(); //删除整页
    void print_page();  //打印整页
    Index& getIndex();
};

class Table::Row{
friend class Index;
/**
 * @brief   [C]数据行 
 * 
 */
private:
    int __Row_Num; //数据行标识
    Index __Index;  //索引
    void** __Content; //数据指针数组(未定义类型)
    Table* t;
public:
    Row(Table* t);  //构造函数
    bool padding(string statement); //内容填充
    char* format(); //格式化转化输出
    void erase();   //内容清空
    Index& getIndex();
    bool operator<(Row&);
    bool operator>(Row&);
    bool operator==(Row&);
    bool operator<=(Row&);
    bool operator>=(Row&);
};





#endif
