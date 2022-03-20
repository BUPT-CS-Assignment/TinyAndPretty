#ifndef __ROW_H__
#define __ROW_H__

class Table::Row{
/**
 * @brief   [C]数据行 
 * 
 */
private:
    int __Row_Num; //数据行标识
    void** __Content; //数据指针数组(未定义类型)
    Table* t;
public:
    Row(Table* t);  //构造函数
    bool padding(string statement); //内容填充
    char* format(); //格式化转化输出
    void erase();   //内容清空
};

#endif