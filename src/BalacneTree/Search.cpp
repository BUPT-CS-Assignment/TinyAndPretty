#include "BalanceTree.h"
/**
 * @brief 数据查找相关函数
 *  
 */

template<class DAT,class Idx>
Node<DAT,Idx>* BalanceTree<DAT,Idx>::insert_node_locate(Idx* idx,Node<DAT,Idx>* start){
    /**
     * @brief 根据索引定位数据查找/插入的叶节点 <parm>索引指针, 起始节点指针
     */
    if(start == NULL) return NULL;
    if(start->__type == __LEAF__) return start;
    int p = start->find_insert_position(idx);
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
    int p = node->find_insert_position(idx);
    if(p == -1) return NULL;
    return node->__data[p];
}

template<class DAT,class Idx>
int Node<DAT,Idx>::find_insert_position(Idx* idx){
    /**
     * @brief 根据索引查找位置 <parm>索引指针
     */
    if(idx == NULL || isFull()) return -1;
    if( __cursor == 0 || *idx <= *__index[0] ) return 0;  
    if(*idx > *__index[__cursor-1]) return __cursor;
    for(int i = 1; i < __cursor; i ++){
        if(*__index[i-1] < *idx && *idx <= *__index[i]){
            return i;
        }
    }
    return -1;
}

template<class DAT,class Idx>
int Node<DAT,Idx>::find_delete_position(Idx* idx){
    for(int i = 0; i < __cursor; i ++){
        if(*idx == *__index[i]){
            return i;
        }
    }
    return -1;
}

template<class DAT,class Idx>
bool Node<DAT,Idx>::find_node(Node<DAT,Idx>* node){
    if(this == node) return true;
    if(__type == __LEAF__) return false;
    for(int i = 0; i <= __cursor; i++){
        if(__child[i]->find_node(node)){
            return true;
        }
    }
    return false;
}


template<class DAT,class Idx>
Idx* BalanceTree<DAT,Idx>::common_index_locate(Node<DAT,Idx>* A,Node<DAT,Idx>* B,Node<DAT,Idx>* start){
    if(start->__type == __LEAF__) return NULL;
    int A_pos = -1, B_pos = -1;
    for(int i = 0; i <= start->__cursor; i++){
        if(start->__child[i]->find_node(A)) A_pos = i; 
        if(start->__child[i]->find_node(B)) B_pos = i; 
    }
    cout<<A_pos<<"/"<<B_pos<<endl;
    if(A_pos == -1 || B_pos == -1) return NULL;
    if(B_pos - A_pos == 1) return start->__index[A_pos];
    if(A_pos == B_pos) return common_index_locate(A,B,start->__child[B_pos]);
    return NULL;
}

