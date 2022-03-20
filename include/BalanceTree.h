#ifndef __BALANCETREE_H__
#define __BALANCETREE_H__
#include<iostream>
using namespace std;
#define __ORDER__ 3
//#include<Implement.h>
/*
    template<class T,class Index>
    [C]BalanceTree - B+树
    [C]Node - 节点类
            [S]Key - 关键字结构
    
*/
/*
    template<class T,class Index>
    [Node]
      index
      *parent
      *left
      *right
      *child[]
      [Key][]
        index
        T* t

*/
enum __NodeType__{__INTERNAL__, __LEAF__};
/*
class Index{
private:
    long long* __ll;
    string* __str;
public:
    Index();
    Index(long long i);
    Index(string str);
    Index(Index* index);
    ~Index();
    bool operator < (Index& index);
    bool operator == (Index& index);
    bool operator > (Index& index);
    //Index& operator= (Index& index);
    friend ostream& operator << (ostream& output,Index& index);
};
*/

template<class DAT,class Idx> class Node;

template<class DAT,class Idx>
class BalanceTree{
    friend class Node<DAT,Idx>;  //节点类
private:
    int __ID__; //树结构标识
    Node<DAT,Idx>* __Data__; //首节点指针
    Node<DAT,Idx>* __Root__; //根节点指针
    Node<DAT,Idx>* locate_node(Idx*,Node<DAT,Idx>* start);  //定位数据插入节点位置
    void insert_adjust(Node<DAT,Idx>*);    //数据插入调整
public:
    BalanceTree(int id);    //树创建
    DAT* search_position(Idx*); //数据查找
    void insert_data(DAT*,Idx*); //数据插入
    void delete_data(Idx*);   //数据删除
    //
    void print_tree();

};

template<class DAT,class Idx>
class Node{
    friend class BalanceTree<DAT,Idx>;  //节点类
private:
    __NodeType__  __type;    //节点类型
    Node<DAT,Idx>* __parent, *__left, *__right;  //父节点指针/左右兄弟节点指针
    Node<DAT,Idx>** __child; //孩子节点指针数组
    Idx** __index; //索引指针数组
    DAT** __data; //数据指针数组
    int __cursor;   //节点数组光标
    //
    Node(__NodeType__ type);//构造函数
    //
    void remove();  //节点删除
    void insert(DAT*,Idx*);  //数据插入
    void insert(Node<DAT,Idx>*,Idx*);   //孩子节点插入
    bool isFull();  //节点是否已满
    //Node<DAT,Idx>* devide(); //节点分裂
    void key_delete(int cursor);    //删除关键字
    int find_position(Idx*);  //定位数据插入位置
    void print_node();
};


#endif