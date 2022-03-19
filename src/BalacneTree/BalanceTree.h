#ifndef __BALANCETREE_H__
#define __BALANCETREE_H__
#include<iostream>
using namespace std;
#define __ORDER__ 5     //B+树阶数
/*

    template<class DAT,class Index>
    [C]BalanceTree - B+树
    [C]Node - 节点类
            [S]Key - 关键字结构
    
*/
enum __NodeType__{__INTERNAL__, __LEAF__};  //节点类型(内部节点, 叶节点)

/**
 * @brief B+树_模板
 * 
 * @tparam DAT 数据指针类型
 * @tparam Idx 数据类型
 */

template<class DAT,class Idx> class Node;

template<class DAT,class Idx>
class BalanceTree{
    friend class Node<DAT,Idx>;  //节点类友元
    /**
     * @brief B+树结构类
     * 
     */
private:
    int __ID__; //树结构标识
    Node<DAT,Idx>* __Data__; //首节点指针
    Node<DAT,Idx>* __Root__; //根节点指针
    Node<DAT,Idx>* insert_node_locate(Idx*,Node<DAT,Idx>* start);  //定位数据插入节点位置
    void insert_adjust(Node<DAT,Idx>*);    //数据插入调整
public:
    BalanceTree(int id);    //树创建
    DAT* search_position(Idx*); //数据查找
    void insert_data(Idx*,DAT*); //数据插入
    void delete_data(Idx*);   //数据删除
    //
    void print_tree();

};

template<class DAT,class Idx>
class Node{
    friend class BalanceTree<DAT,Idx>;  //树结构类友元
    /**
     * @brief B+树节点结构类
     * 
     */
private:
    __NodeType__  __type;    //节点类型
    Node<DAT,Idx>* __parent, *__left, *__right;  //父节点指针/左右叶子节点指针
    Node<DAT,Idx>** __child; //孩子节点指针数组
    Idx** __index; //索引指针数组
    DAT** __data; //数据指针数组
    int __cursor;   //索引数组光标
    //
    Node(__NodeType__ type);//构造函数(指定节点类型)
    //
    void remove();  //节点删除
    void insert(Idx*, DAT*);  //数据插入
    void insert(Idx*, Node<DAT,Idx>*);   //孩子节点插入
    bool isFull();  //节点是否已满
    Node<DAT,Idx>* node_divide(int first_position);    //节点分裂
    //void key_delete(int cursor);    //删除关键字
    int find_position(Idx*);  //定位数据插入位置
    void print_node();
};


#endif