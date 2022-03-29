#ifndef __DATA_H__
#define __DATA_H__
#include<Basic.h>
#include<BalanceTree.h>
#include<Storage.h>

class Index{
    friend class Table;
    friend class Row;
    friend class Memorizer;
    INDEX       index_;
    DATA_TYPE    type_;
public:
    Index(string index, DATA_TYPE);
    Index(DATA_TYPE);
    Index(int);
    Index(long long);
    Index(double);
    Index(string);
    Index();
    int getSize();
    bool operator<(Index &);
    bool operator>(Index &);
    bool operator==(Index &);
    bool operator<=(Index &);
    bool operator>=(Index &);
    friend ostream &operator << (ostream &out, Index &index);
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
    friend class Analyzer;
    struct ePage{
        __uint16_t offset;
        ePage* next;
        ePage(){offset = -1; next = NULL;}
    };
private:
    //FileHead
    int             table_id_;      //表ID索引
    char            table_name_[32] = {0};    //表名
    //TableHead
    __uint16_t      max_offset;    //页总数
    //__uint16_t      max_pages;
    //__uint16_t      empty_pages_[1 + MAX_EMPTY_PAGE] = {0};    //记录空页
    __uint16_t      parm_num_;      //参数总数
    DATA_TYPE *parm_types_;     //数据类型数组
    char(*parm_names_)[32];     //数据元素名称数组 
    __uint16_t      prim_key_;          //索引位置
    __uint16_t      row_take_up_;    //单行字节数
    __uint16_t      max_rows_per_page_;//单页最大行数 
    //////////////////////////////////////////////
    ePage empty_pages;   
    //TableBody
    //In-Memory
    BalanceTree<__uint16_t, Index> *pages_tree_;   //索引B+树
    BalanceTree<Index, Index> **index_tree_;//非主键索引
    /////////////////////////////////////////////////////
    Page *page_locate(Index);       //数据页搜索
    ///////////////////////////////////////////////////////
    
    //INSERT
    //bool check_empty(int);
    __uint16_t  get_empty_page_offset();
    void add_empty_page(__uint16_t);
    //SELECT
    string select_by_key(Analyzer&);
    string select_by_traverse(Analyzer&);
    //DELETE
    bool delete_by_key(Analyzer&);
    bool delete_by_traverse(Analyzer&);
    //UPDATE
    bool update_by_key(Analyzer&,Analyzer&);
    bool update_by_traverse(Analyzer&,Analyzer&);
    //bool remove_table();            //删除表
    ////////////////////////////////////////////////////////////
public:
    Table(int id, string name);     //构造函数
    bool Init(string statement);    //初始化表数据类型
    /**
     *数据操作相关
     */
     //INSERT
    int ParmLocate(string name);  //Mapping
    bool InsertValues(string conditions, string values);//条件插入
    //DELETE
    bool DeleteValues(string conditions);  //条件删除
    //SELECT
    string SelectValues(string conditions, string values);
    //UPDATE
    bool UpdateValues(string condition, string values);
    /////////
    void print_table();             //打印表
    void print_structure();         //打印表结构
    //DATA_TYPE* getDataType(); //获取数据元素类型
    string getName();               //获取表名
    DATA_TYPE getKeyType();
    bool delete_traversal(DATA_TYPE, string);
    
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
    //File Head
    bool            not_empty_;      //页空信息
    Index           page_index_;    //页索引
    bool            is_full_;       //已满信息
    //Page Head
    __uint16_t      cursor_pos_; //行光标偏移量
    //In-Memory
    Row **rows_;   //行指针数组
    Table *table_ptr_;       //表指针
    //////////////////////////////////////
    Page(Table *table_ptr_); //构造函数
    //INSERT
    bool InsertRow(Row *new_row); //插入行
    //SELECT
    string SelectRow(Analyzer&);
    //UPDATE
    bool UpdateRow(Analyzer&,Analyzer&);
    //DELETE
    bool DeleteRow(Analyzer&);
    //Clear
    void Clear(__uint16_t);
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
    friend class Page;
    friend class Analyzer;
    /**
     * @brief   [C]数行 
     *
     */
private:
    //RowHaed
    Index row_index_;
    //RowBody
    void **content_; //数据指针数组(未定义类型)
    //In-Memory
    Table *table_ptr_;
public:
    Row(Table *table_ptr_);  //构造函数
    bool Padding(string conditions, string values); //内容填充
    string Format(); //格式化转化输出
    string get_value(int n);
    string get_values(int[], int n);
    bool update_values(int[],string[],int);
    bool update_value(int,string);
    void Erase();   //内容清空
    Index &getIndex();
    void index_update();
    bool operator<(Row &);
    bool operator>(Row &);
    bool operator==(Row &);
    bool operator<=(Row &);
    bool operator>=(Row &);
};

class Analyzer{
friend class Row;
private:
    //condition match
    Table* table_ptr_;
    int cond_num;
    int* cond_pos;
    char* cond_cmp;
    Index* cond_val;
    string* cond_origin;
    //value match
    int parm_num;
    int* parm_pos;
    //prim key support
    int key_pos;

public:
    Analyzer(Table* table);
    bool Extract(string, string);
    bool Match(Row* row);
    bool Locate(string params);
    int* getParmPos(){return parm_pos;}
    int getParmNum(){return parm_num;}
    bool KeySupport(){return key_pos>=0;}
    int getKeyPos(){return key_pos;}
    Index* getCondVal(int i){
        if(i>=cond_num){
            return NULL;
        }
        return &cond_val[i];
    }
    int* getCondPos(){return cond_pos;}
    int getCondNum(){return cond_num;}
    string* getCondOrigin(){return cond_origin;}

};


#endif
