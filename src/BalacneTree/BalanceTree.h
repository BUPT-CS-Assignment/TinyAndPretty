#ifndef __BALANCETREE_H__
#define __BALANCETREE_H__
#include<iostream>
using namespace std;
#define __ORDER__ 5     //B+树阶数
/**
 * @brief B+树头文件
 * 
 * 结构:
 * 
 *  template<class DAT,class Index>
 *  [C]BalanceTree - B+树
 *  [C]Node - 节点类
 *      [A]__index - 索引数组
 *      [A]__data - 数据指针数组
 *      [A]__child - 子节点数组  
 * 
 */
enum __NodeType__{__INTERNAL__, __LEAF__};  //节点类型(内部节点, 叶节点)

/**
 * @brief B+树_模板
 * 
 * @tparam DAT_数据指针类型
 * @tparam Idx_索引类型
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
    Idx* common_index_locate(Node<DAT,Idx>*,Node<DAT,Idx>*,Node<DAT,Idx>*); //寻找共同祖先索引
    void delete_adjust(Node<DAT,Idx>*); //删除节点后进行树结构调整
    Node<DAT,Idx>* node_merge(Node<DAT,Idx>*,Idx*, Node<DAT,Idx>*); //拉取父索引后合并
public:
    BalanceTree(int id);    //树创建
    DAT* search_position(Idx*); //数据查找
    void insert_data(Idx*,DAT*); //数据插入
    void delete_data(Idx*);   //数据删除
    void check_all();   //遍历树
    DAT* getHeader();   //获取首个数据节点
    void print_tree();  //打印整树

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
    bool isSatisfied(); //关键字数量是否满足阶数要求
    bool isMerge(Node<DAT,Idx>*);   //节点能够与另一节点合并
    bool isLend(); //节点是否可借
    Node<DAT,Idx>* divide();    //节点分裂
    //void key_delete(int cursor);    //删除关键字
    int find_insert_position(Idx*);  //定位数据插入位置
    int find_delete_position(Idx*); //定位数据删除位置
    bool delete_data(Idx*); //根据索引删除数据指针
    Node<DAT,Idx>* left_merge(Node<DAT,Idx>*);    //与左节点合并
    void right_merge(Node<DAT,Idx>*);    //与右节点合并
    void left_lend(Idx*);   //从左节点借
    void right_lend(Idx*);  //从右节点借
    void print_node();  //打印节点数据
    void delete_index(int); //根据位置抹除索引及子节点
    bool find_node(Node<DAT,Idx>*); //寻找节点
    
    
};


#endif