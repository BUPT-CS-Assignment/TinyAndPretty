#include "BalanceTree.h"
/**
 * @brief 数据删除相关函数
 *  
 */

template<class DAT,class Idx>
void BalanceTree<DAT,Idx>::insert_data(Idx* idx,DAT* data){
    /**
     * @brief 插入数据单元 <parm>数据指针, 索引指针
     */
    //根节点为空, 创建根节点
    if(__Root__ == NULL){
        __Root__ = new Node<DAT,Idx> ( __LEAF__ );
        __Root__ -> insert(data,idx);
        __Data__ = __Root__;
        return;
    }
    Node<DAT,Idx>* node = insert_node_locate(idx,__Root__);
    node->insert(data,idx);
    insert_adjust(node);
}

template<class DAT,class Idx>
void BalanceTree<DAT,Idx>::insert_adjust(Node<DAT,Idx>* node){
    /**
     * @brief 树结构调整 <parm>需调整节点指针
     */
    if(node == NULL || ! node->isFull()) return;
    Node<DAT,Idx>* resource = node->__parent;   //定位父亲节点
    Idx* new_idx = new Idx(*node->__index[__ORDER__/2]);  //确定新索引
    //节点分裂
    Node<DAT,Idx>* new_node = node->divide();
    //父节点操作
    if(resource == NULL){
        resource = new Node<DAT,Idx>(__INTERNAL__); //新建父节点索引
        resource -> __child[0] = node;
        node->__parent = resource;
    }
    resource->insert(new_idx,new_node);
    __Root__ = (node == __Root__?resource : __Root__);  //更新根节点指针
    //向上递归调整树结构
    insert_adjust(resource);
}

template<class DAT,class Idx>
void Node<DAT,Idx>::insert(Idx* idx,DAT* data){
    /**
     * @brief 插入数据指针 <parm>索引指针, 数据指针
     */
    if(isFull() || idx == NULL) return;
    //找到需要插入的位置
    int insert_position = find_insert_position(idx);
    //原数据/索引后移
    for(int i = __cursor; i > insert_position; i--){
        __index[i] = __index[i-1];
        __data[i] = __data[i-1];
    }
    //插入数据/索引
    __data[insert_position] = data;
    __index[insert_position] = idx;
    __cursor ++ ;
}

template<class DAT,class Idx>
void Node<DAT,Idx>::insert(Idx* idx,Node<DAT,Idx>* node){
    /**
     * @brief 插入孩子节点指针 <parm>索引指针, 节点指针
     */
    if(isFull()) return;
    int insert_position = find_insert_position(idx);
    for(int i = __cursor; i > insert_position; i--){
        __index[i] = __index[i-1];
        __child[i+1] = __child[i];
    }
    __index[insert_position] = idx;
    __child[insert_position + 1] = node;
    node->__parent = this;  //更新父节点指针
    __cursor ++ ;   //更新光标
}

