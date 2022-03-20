#include "BalanceTree.h"
/**
 * @brief BalanceTree类基本函数
 *  
 */


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
template<class DAT,class Idx>
DAT* BalanceTree<DAT,Idx>::getHeader(){
    if(__Data__ == NULL) return NULL;
    return __Data__->__data[0];
}

template<class DAT,class Idx>
void BalanceTree<DAT,Idx>::check_all(){
    if(__Data__ == NULL){
        cout<<" <Tree Empty> "<<endl;
        return;
    }
    Node<DAT,Idx>* node = __Data__;
    while(node != NULL){
        for(int i = 0;i < node->__cursor; i++){
            cout<<*node->__data[i]<<" -> ";
        }
        node = node->__right;
    }
    cout<<"<end>"<<endl;
}

