#ifndef __PAGE_H__
#define __PAGE_H__
class Table::Page{
public:
    int __Page_Num;    //页码索引
    int __Cursor;   //行光标
    Row** __Rows;   //行指针数组
    Table* t;
public:
    Page(int page_num,Table* t); //构造函数
    bool isFull();  //判断满页
    int getCursor();    //获取光标位置
    void insert(Row* new_row); //插入行
    bool delete_row(int row_number);    //按行号删除
    bool remove_page(); //删除整页
    void print_page();  //打印整页
};

#endif