#include "BalanceTree.h"
/**
 * @brief 数据删除相关函数
 *  
 */


template<class DAT,class Idx>
void BalanceTree<DAT,Idx>::delete_data(Idx* idx){
    Node<DAT,Idx>* node = insert_node_locate(idx,__Root__);
    if(node == NULL) return;
    if(!node->delete_data(idx)) return;
    if(node->isSatisfied()) return;
    //合并节点优先///////////////////////////////////////////////////////////////////
    if(node->__left !=NULL && node->isMerge(node->__left)){
        //左合并
        node = node->left_merge(node->__left);
        delete_adjust(node->__parent);
    }else if(node->__right != NULL && node->isMerge(node->__right)){
        //右合并
        node->right_merge(node->__right);
        delete_adjust(node->__parent);
    }else if(node->__left != NULL && node->__left->isLend()){ //左节点可借
        node->left_lend(NULL);
        Idx* common_idx = common_index_locate(node->__left,node,__Root__); //找到共同父索引
        *common_idx = *(new Idx(*node->__index[0]));    //更新共同父索引

    }else if(node->__right != NULL && node->__right->isLend()){ //右节点可借
        node->right_lend(NULL);
        Idx* common_idx = common_index_locate(node,node->__right,__Root__); //找到共同父索引
        *common_idx = *(new Idx(*node->__right->__index[0])); //更新共同父索引
    }
    /* 借节点优先/////////////////////////////////////////////////////////////
    if(node->__left != NULL && node->__left->isLend()){ //左节点可借
        node->left_lend(NULL);
        Idx* common_idx = common_index_locate(node->__left,node,__Root__); //找到共同父索引
        *common_idx = *(new Idx(*node->__index[0]));    //更新共同父索引

    }else if(node->__right != NULL && node->__right->isLend()){ //右节点可借
        node->right_lend(NULL);
        Idx* common_idx = common_index_locate(node,node->__right,__Root__); //找到共同父索引
        *common_idx = *(new Idx(*node->__right->__index[0])); //更新共同父索引

    }else{
        if(node->__left !=NULL && node->__left->__parent == node->__parent){
            //左合并
            node = node->left_merge(node->__left);
        }else if(node->__right != NULL && node->__right->__parent == node->__parent){
            //右合并
            node->right_merge(node->__right);
        }
        delete_adjust(node->__parent);
    }
    */
}



template<class DAT,class Idx>
bool Node<DAT,Idx>::delete_data(Idx* idx){
    /**
     * @brief 删除索引指向的数据指针 <parm>索引指针
     */
    int p = find_delete_position(idx);
    if(p == -1) return false;
    delete[] __data[p];
    delete[] __index[p];
    for(int i = p; i<__cursor; i++){
        __data[i] = __data[i+1];
        __index[i] = __index[i+1];
    }
    __cursor --;
    return true;
}

template<class DAT,class Idx>
void Node<DAT,Idx>::delete_index(int p){
    /**
     * @brief 合并节点后删除父节点索引 <parm>删除索引位置
     */
    if(p == -1) return;
    for(int i = p; i<__cursor; i++){
        __index[i] = __index[i+1];
        __child[i+1] = __child[i+2];
    }
    __cursor --;
}


template<class DAT,class Idx>
void BalanceTree<DAT,Idx>::delete_adjust(Node<DAT,Idx>* node){
    /**
     * @brief 删除后树结构调整 <parm>起始节点指针
     * 
     */
    if(node->isSatisfied()) return; //满足树结构无需调整
    if(node->__left != NULL && node->__left->isLend()){ //左节点可借
        //拷贝原索引
        Idx* temp_idx = new Idx(*node->__index[0]);
        Idx* common_idx = common_index_locate(node->__left,node,__Root__); //找到共同父索引
        node->left_lend(common_idx);   //当前节点索引更新
        *common_idx = *temp_idx;    //更新共同父索引
        return;

    }else if(node->__right != NULL && node->__right->isLend()){ //右节点可借
        //拷贝原索引
        Idx* temp_idx = new Idx(*node->__right->__index[0]);
        Idx* common_idx = common_index_locate(node,node->__right,__Root__); //找到共同父索引
        node->right_lend(common_idx);  //当前节点索引更新
        *common_idx = *temp_idx;    //更新共同父索引
        return;
    }else{
        if(node->__left !=NULL && node->__left->__parent == node->__parent){
            //左合并
            //定位父索引
            int p = node->__parent->find_insert_position(node->__left->__index[node->__left->__cursor-1]);
            Idx* temp_idx = new Idx(*node->__parent->__index[p]);   //拉取父索引
            node = node_merge(node->__left,temp_idx,node);  //合并
            node->__parent->delete_index(p);    //删除父索引
        }else if(node->__right != NULL && node->__right->__parent == node->__parent){
            //右合并
            int p = node->__parent->find_insert_position(node->__index[node->__cursor-1]);  //定位父索引
            Idx* temp_idx = new Idx(*node->__parent->__index[p]);   //拉取父索引
            node = node_merge(node,temp_idx,node->__right); //合并
            node->__parent->delete_index(p);    //删除父索引
        }
        if(node->__parent == __Root__ &&  __Root__ -> __cursor == 0){
            __Root__ = node;    //更新根节点
            return;
        }
    }
    if(node->__parent == __Root__) return;
    delete_adjust(node->__parent);  //递归调整判断
}

/////////////////////////////////////////////////////////////////


template<class DAT,class Idx>
Node<DAT,Idx>* BalanceTree<DAT,Idx>::node_merge(Node<DAT,Idx>* nodeA, Idx* idx,Node<DAT,Idx>* nodeB){
    /**
     * @brief 拉取父节点索引后合并 <parm>左节点指针, 父索引, 右节点指针
     */
    nodeA->__index[nodeA->__cursor++] = new Idx(*idx);  //加入父索引
    //节点数据迁移
    for(int i = 0; i < nodeB->__cursor; i++){
        nodeA->__index[nodeA->__cursor ++] = nodeB->__index[i];
        nodeA->__child[nodeA->__cursor - 1] = nodeB->__child[i];
        nodeB->__child[i]->__parent = nodeA;
    }
    nodeA->__child[nodeA->__cursor] = nodeB->__child[nodeB->__cursor];
    nodeB->__child[nodeB->__cursor]->__parent = nodeA;
    //节点间指针更新
    nodeA->__right = nodeB->__right;
    if(nodeB->__right!=NULL)    nodeB->__right->__left = nodeA;
    nodeB->remove();
    return nodeA;
}



template<class DAT,class Idx>
Node<DAT,Idx>* Node<DAT,Idx>::left_merge(Node<DAT,Idx>* node){
    /**
     * @brief 左节点合并 <parm>左节点指针
     */
    int p = __parent->find_insert_position(__index[0]);
    for(int i = 0; i < __cursor; i++){
        node->insert(__index[i],__data[i]);
        __data[i] = NULL;
        __index[i] = NULL;
    }
    node->__right = __right;
    if(node->__left != NULL) node->__left->__right = node;
    __left = __right = __parent = NULL;
    node->__parent->delete_index(p);
    return node;
}

template<class DAT,class Idx>
void Node<DAT,Idx>::right_merge(Node<DAT,Idx>* node){
    /**
     * @brief 右节点合并 <parm>右节点指针
     */
    int p = node->__parent->find_insert_position(node->__index[0]);
    for(int i = 0; i < node->__cursor; i++){
        this->insert(node->__index[i],node->__data[i]);
        node->__data[i] = NULL;
        node->__index[i] = NULL;
    }
    if(node->__right != NULL) node->__right->__left = this;
    this->__right = node->__right;
    node->__left = node->__right = node->__parent = NULL;
    __parent->delete_index(p);
}

template<class DAT,class Idx>
void Node<DAT,Idx>::left_lend(Idx* new_idx){
    /**
     * @brief 向左节点借数据 <parm> 本节点更新后的索引
     */
    Node<DAT,Idx>* left = this->__left;
    if(__type == __INTERNAL__){
        for(int i = __cursor; i > 0; i--){
            __index[i] = __index[i-1];
            __child[i+1] = __child[i]; 
        }__child[1] = __child[0];
        __child[0] = left->__child[left->__cursor];
        __child[0]->__parent = this;
        __index[0] = new Idx(*new_idx);

    }else if(__type == __LEAF__){
        for(int i = __cursor; i > 0; i--){
            __index[i] = __index[i-1];
            __data[i] = __data[i-1]; 
        }
        __data[0] = left->__data[left->__cursor - 1];
        __index[0] = new Idx(*left->__index[left->__cursor - 1]);
    }
    left->__cursor -- ;
    __cursor ++ ;
    
}

template<class DAT,class Idx>
void Node<DAT,Idx>::right_lend(Idx* new_idx){
    /**
     * @brief 向右节点借数据 <parm> 本节点更新后的索引
     */
    Node<DAT,Idx>* right = this->__right;
    if(__type == __INTERNAL__){
        __child[__cursor+1] = right->__child[0];
        __child[0]->__parent = this; 
        for(int i = 0; i < right->__cursor; i++){
            if(i < right->__cursor -1 ){
                right->__index[i] = right->__index[i+1];
            }
            right->__child[i] = right->__child[i+1]; 
        }
        __index[__cursor] = new Idx(*new_idx);

    }else if(__type == __LEAF__){
        __data[__cursor] = right->__data[0];
        __index[__cursor] = new Idx(*right->__index[0]);
        for(int i = 0; i < right->__cursor - 1; i++){
            right->__index[i] = right->__index[i+1];
            right->__data[i] = right->__data[i+1]; 
        }
        
    }
    right->__cursor -- ;
    __cursor ++ ;
}