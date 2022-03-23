#ifndef __DATA_H__
#define __DATA_H__
#include<Basic.h>
#include<BalanceTree.h>
#include<Storage.h>

class Index{
friend class Table;
friend class Row;
friend class Memorizer;
    __INDEX__       INDEX;
    __DataType__    index_type;
public:
    Index(string index,__DataType__);
    Index(__DataType__);
    Index();
    bool operator<(Index&);
    bool operator>(Index&);
    bool operator==(Index&);
    bool operator<=(Index&);
    bool operator>=(Index&);
    friend ostream & operator << (ostream & out, Index & index);
};

/**
 * @brief 数据表Table类
 * 
 * 表结构
 * ----------------------
 * FileHaed   表信息
 *            文件指针头
 * ----------------------
 * TableHead  表数据数
 *            表数据类型:ID+Type
 *            表主键
 *            总页数
 * ----------------------
 * TableBody  B+树
 *            页偏移量+页索引
 * ----------------------
 * FileTail   TAIL标识
 * ----------------------
 * 
 */


class Table{
friend class Page;
friend class Row;
friend class Index;
friend class Memorizer;
friend class Executor;
private: 
    //FileHead
    int             __TableID;      //表ID索引
    string          __TableName;    //表名
    __uint16_t      __TableHeadSize;//表头数据量
    //TableHead
    __uint16_t     __TotalPage;    //页总数
    __uint16_t     __EmptyPage[1+MAX_EMPTY_PAGE];    //记录空页
    __uint16_t       __ParmNum;      //参数总数
    __DataType__*   __ParmType;     //数据类型数组
    string*         __ParmName;     //数据元素名称数组 
    __uint16_t       __Key;          //索引位置
    __uint16_t      __RowTakeup;    //单行字节数
    __uint16_t       __MaxRowPerPage;//单页最大行数
    //TableBody
    
    //In-Memory
    BalanceTree<__uint16_t, Index>*  __Pages;   //索引B+树
    Page* page_locate(Index);       //数据页搜索
public:
    Table(int id, string name);     //构造函数
    bool init(string statement);    //初始化表数据类型
    bool check_empty(int);
    __uint16_t  get_empty_page_offset();
    void add_empty_page(__uint16_t);
    /**
     *数据操作相关
     */
    bool insert(string statement);  //数据行插入
    void add_page(Page* Node);      //添加页
    bool delete_page(Index&);       //删除页
    bool delete_row(Index&);        //删除指定行
    bool remove_table();            //删除表
    void print_table();             //打印表
    void print_structure();         //打印表结构
    //
    //__DataType__* getDataType(); //获取数据元素类型
    string getName();               //获取表名
    __DataType__ getKeyType();
};


/**
 * @brief 数据页Page类
 * 
 * 页结构
 * ----------------------
 * FileHaed   页索引
 * ----------------------
 * PageHead  页已满信息
 *           页数据最小值
 *           页数据最大值
 *           页数据量
 *           页数据光标位置
 * ----------------------
 * PageBody  行数据
 * ----------------------
 * FileTail  TAIL标识
 *           下页文件指针偏移量
 * ----------------------
 * 
 */

class Page{
friend class Index;
friend class Table;
friend class Row;
friend class Memorizer;
public:
    //File Head
    __uint16_t      __PageOffset;   //页偏移量
    Index           __PageIndex;    //页索引
    //Page Head
    bool            __IsFull;       //已满信息
    __uint16_t      __CursorOffset; //行光标偏移量
    //In-Memory
    Row** __Rows;   //行指针数组
    Table* t;       //表指针
public:
    Page(Table* t); //构造函数
    Page(__uint16_t,Table*);
    bool insert(Row* new_row); //插入行
    bool delete_row(Index& index);  //按索引删除
    bool remove_page(); //删除整页
    void print_page();  //打印整页
};

/**
 * @brief 数据行Row类
 * 
 * 行结构
 * ----------------------
 * RowHaed   行索引
 * ----------------------
 * RowBody   行数据
 * ----------------------
 * 
 */

class Row{
friend class Index;
friend class Memorizer;
/**
 * @brief   [C]数据行 
 * 
 */
private:
    //RowHaed
    Index __RowIndex;
    //RowBody
    void** __RowContent; //数据指针数组(未定义类型)
    //In-Memory
    Table* t;
public:
    Row(Table* t);  //构造函数
    bool padding(string statement); //内容填充
    char* format(); //格式化转化输出
    void erase();   //内容清空
    Index& getIndex();
    void index_update();
    bool operator<(Row&);
    bool operator>(Row&);
    bool operator==(Row&);
    bool operator<=(Row&);
    bool operator>=(Row&);
};


#endif
