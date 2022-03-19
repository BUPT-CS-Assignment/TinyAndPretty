#include "BalanceTree.h"

template<class DAT,class Idx>
Node<DAT,Idx>* BalanceTree<DAT,Idx>::insert_node_locate(Idx* idx,Node<DAT,Idx>* start){
    /**
     * @brief 根据索引定位数据查找/插入的叶节点 <parm>索引指针, 起始节点指针
     */
    if(start == NULL) return NULL;
    if(start->__type == __LEAF__) return start;
    int p = start->find_position(idx);
    if(p == -1) return NULL;
    return insert_node_locate(idx,start->__child[p]);
}

template<class DAT,class Idx>
DAT* BalanceTree<DAT,Idx>::search_position(Idx* idx){
    /**
     * @brief 根据索引查找数据单元位置 <parm>索引指针
     */
    Node<DAT,Idx>* node = insert_node_locate(idx,__Root__);
    if(node == NULL) return NULL;
    int p = node->find_position(idx);
    if(p == -1) return NULL;
    return node->__keys[p];
}

template<class DAT,class Idx>
int Node<DAT,Idx>::find_position(Idx* idx){
    /**
     * @brief 根据索引查找位置 <parm>索引指针
     */
    if(idx == NULL || isFull()) return -1;
    if( __cursor == 0 || *idx < *__index[0] ) return 0;  
    if(*__index[__cursor-1] < *idx) return __cursor;
    for(int i = 1; i < __cursor; i ++){
        if(*__index[i-1] < *idx && *idx < *__index[i]){
            return i;
        }
    }
    return -1;
}


