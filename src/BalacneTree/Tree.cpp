#include "BalanceTree.h"

template<class DAT,class Idx>
BalanceTree<DAT,Idx>::BalanceTree(int id){
    __ID__ = id;
    __Data__ = NULL;
    __Root__ = NULL;
}

template<class DAT,class Idx>
void BalanceTree<DAT,Idx>::print_tree(){
    /**
     * @brief 打印树结构
     */
    __Root__->print_node();
}

    

