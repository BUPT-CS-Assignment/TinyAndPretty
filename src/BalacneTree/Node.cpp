#include "BalanceTree.h"

template<class DAT,class Idx>
Node<DAT,Idx>::Node(__NodeType__ type){
    /**
     * @brief 节点类构造函数 <parm>节点类型
     */
    __type = type;
    __parent = __left =  __right = NULL;
    __index = new Idx*[__ORDER__ ];
    //判断节点类型进行内存分配
    __data = (type == __LEAF__? new DAT*[__ORDER__ ] : NULL);
    __child = (type == __INTERNAL__? new Node<DAT,Idx>*[__ORDER__ + 1] : NULL);
    __cursor = 0;
}

template<class DAT,class Idx>
bool Node<DAT,Idx>::isFull(){
    /**
     * @brief 判断节点是否已满
     */
    return __cursor >= __ORDER__; 
}

template<class DAT,class Idx>
Node<DAT,Idx>* Node<DAT,Idx>::node_divide(int div_position){
    /**
     * @brief 根据参数位置分裂节点 <parm>分裂位置
     */
    Node<DAT,Idx>* new_node = NULL;
    if(this->__type == __LEAF__){
        new_node = new Node<DAT,Idx>(__LEAF__); //新增叶节点
        //数据转移
        for(int i = div_position; i < __cursor; i++){
            new_node->insert(__index[i], __data[i]);
            __data[i] = NULL;
        }
        //调整叶节点间指针
        new_node->__right = __right;
        new_node->__left = this;
        if(__right != NULL)   __right->__left = new_node;
        __right = new_node;
        
    }else if(this->__type == __INTERNAL__){
        new_node = new Node<DAT,Idx>(__INTERNAL__); //新增内部节点
        //数据转移(提取中间索引)
        //最左侧节点指针初始化
        new_node->__child[0] = __child[div_position + 1];
        new_node->__child[0]->__parent = new_node;
        __child[div_position + 1] = NULL;
        //节点/孩子节点指针插入
        for(int i = div_position + 1; i < __cursor; i++){
            new_node->insert(__index[i],__child[i+1]);
            __child[i+1] = NULL;
        }
    }
    __cursor = div_position;   //更新原节点光标
    return new_node;
}


template<class DAT,class Idx>
void Node<DAT,Idx>::remove(){
    /**
     * @brief 移除节点
     */
    if(__type == __INTERNAL__){
        for(int i = 0; i< __cursor + 1; i++)    __child[i] = NULL;
    }else{
        for(int i = 0; i < __cursor; i++)   __index[i] = __data[i] = NULL;

    }
    delete __data;  delete __child; delete __index;
    if(__left != NULL) __left ->__right = this->__right;
    __parent = __left = __right = NULL;   
}

template<class DAT,class Idx>
void Node<DAT,Idx>::print_node(){
    /**
     * @brief 打印节点索引信息
     */
    for(int i = 0;i < __cursor; i++){
        cout<<*(__index[i])<<" ";
    }cout<<endl;    
}



