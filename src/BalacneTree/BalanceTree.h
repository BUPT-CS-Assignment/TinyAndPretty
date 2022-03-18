#ifndef __BALANCETREE_H__
#define __BALANCETREE_H__
#include<iostream>
using namespace std;
#define __ORDER__ 3
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
enum __NodeType__{__ROOT__, __INTERNAL, __LEAF__};
template<class T> class Node;
class Index;

template<class T>
class BalanceTree{
    friend class Node<T>;  //节点类
    friend class Index; //索引类
private:
    int __ID__; //树结构标识
    Node<T>* __Data__; //首节点指针
    Node<T>* __Root__; //跟节点指针
public:
    BalanceTree(int id);    //树创建
    T* search_data(Index& index); //数据查找
    void insert_data(T* t,Index& index); //数据插入
    Node<T>* locate_nodex(Index& index,Node<T>* start);  //定位数据插入节点位置
    void delete_data(Index& index);   //数据删除


};

template<class T>
class Node{
private:
    __NodeType__ __type;    //节点类型
    Index __index;  //节点索引
    Node<T>* __parent, *__left, *__right;  //父节点指针/左右兄弟节点指针
    Node<T>** __child; //孩子节点指针数组
    struct Key{
        T* t;   //数据单元指针
        Index index;    //关键字索引
        Key(Index& index,T* t = NULL){
            this-> t = NULL;
            this->index = index; 
        }
        Key(){this->t = NULL;}
    };
    Key* __keys;    //关键字数组
    int __cursor;   //关键字数组光标
public:
    Node();//构造函数
    //
    void remove();  //节点删除
    void insert(T* t,Index& index);  //数据插入
    bool isFull();  //节点是否已满
    Node<T>* devide(); //节点分裂
    void key_delete(int cursor);    //删除关键字
    int find_insert_position(Index& index);  //定位数据插入位置
};


class Index{
private:
    long long* __ll;
    string* __str;
public:
    Index(long long i);
    Index(string str);
    bool operator < (Index& index);
    bool operator == (Index& index);
    bool operator > (Index& index);
    Index& operator= (Index& index);

};


#endif