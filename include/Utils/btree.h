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
 *      [A]__child - 子节数组  
 *  [S]DataNode - 数据指针迭代器
 *
 */

enum __NodeType__{
    __INTERNAL__, __LEAF__
};  //节点类型(内部节点, 叶节点)

/**
 * @brief B+树_模板
 *
 * @tparam DAT_数据指针类型
 * @tparam Idx_索引类型
 */



template<class DAT, class Idx> class Node;

template<class DAT, class Idx>
struct DataNode{
    Node<DAT, Idx> *node;
    int pos;
    DataNode<DAT, Idx>();
    DAT* getData();
    DataNode<DAT, Idx> &operator ++();
    DataNode<DAT, Idx> &operator --();
};

template<class DAT, class Idx>
class BalanceTree{
    friend class Node<DAT, Idx>;  //节点类友元
    friend struct DataNode<DAT, Idx>;
    /**
     * @brief B+树结构类
     *
     */
private:
    int __ID__; //树结构标识
    Node<DAT, Idx> *__Data__; //首节点指针
    Node<DAT, Idx> *__Root__; //根节点指针
    Node<DAT, Idx> *find_insert_leaf_node(Idx&, Node<DAT, Idx> *start);  //定位数据插入节点位置
    void insert_adjust(Node<DAT, Idx> *);    //数据插入调整
    Idx* common_index_locate(Node<DAT, Idx> *, Node<DAT, Idx> *, Node<DAT, Idx> *); //寻找共同祖先索引
    void delete_adjust(Node<DAT, Idx> *); //删除节点后进行树结构调整
    Node<DAT, Idx> *node_merge(Node<DAT, Idx> *, Idx&, Node<DAT, Idx> *); //拉取父索引后合并
    //
    Node<DAT, Idx> *findHead(Node<DAT, Idx> *node){
        if(node == NULL) return NULL;
        if(node->__type == __LEAF__) return node;
        return findHead(node->__child[0]);
    }
public:
    BalanceTree(int id);    //树创建
    DataNode<DAT, Idx> LocateData(Idx&); //数据查找
    DAT  AccurateLoacte(Idx&); //数据查找
    void InsertData(Idx&, DAT&); //数据插入
    void DeleteData(Idx&);   //数据删除
    //void UpdateData(Idx*,DAT*);
    void CheckAll();   //遍历树
    DataNode<DAT, Idx> getLink();
    void CutDown();

};

/**
 * @brief B+树节点结构类
 *
 */
template<class DAT, class Idx>
class Node{
    friend class BalanceTree<DAT, Idx>;  //树结构类友元
    friend struct DataNode<DAT, Idx>;
private:
    __NodeType__  __type;    //节点类型
    Node<DAT, Idx> *__parent, *__left, *__right;  //父节点指针/左右叶子节点指针
    Node<DAT, Idx> **__child; //孩子节点指针数组
    Idx *__index; //索引指针数组
    DAT *__data; //数据指针数组
    int __cursor;   //索引数组光标
    //
    Node(__NodeType__ type);//构造函数(指定节点类型)
    void erase();
    //
    void remove();  //节点删除
    void insert(Idx&, DAT);  //数据插入
    void insert(Idx&,Node<DAT, Idx> *);   //孩子节点插入
    bool isFull();  //节点是否已满
    bool isSatisfied(); //关键字数量是否满足阶数要求
    bool isMerge(Node<DAT, Idx> *);   //节点能够与另一节点合并
    bool isLend(); //节点是否可借
    Node<DAT, Idx> *divide();    //节点分裂
    //void key_delete(int cursor);    //删除关键字
    int find_insert_position(Idx&);  //定位数据插入位置
    int find_delete_position(Idx&); //定位数据删除位置
    bool DeleteData(Idx&); //根据索引删除数据指针
    Node<DAT, Idx> *left_merge(Node<DAT, Idx> *);    //与左节点合并
    void right_merge(Node<DAT, Idx> *);    //与右节点合并
    void left_lend(Idx&);   //从左节点借
    void right_lend(Idx&);  //从右节点借
    void delete_index(int); //根据位置抹除索引及子节点
    bool find_node(Node<DAT, Idx> *); //寻找节点
    ///////////////////////////
public:
    void print_node();  //打印节点数据
    int get_cursor(){
        return __cursor;
    }  //返回光标
    Node<DAT, Idx> *get_side(int);
};

///////////////////////////////////////////////////////////////////////////////

/**
 * @brief BalanceTree类基本函数
 *
 */

template<class DAT, class Idx>
BalanceTree<DAT, Idx>::BalanceTree(int id){
    __ID__ = id;
    __Root__ = NULL;
}


template<class DAT, class Idx>
void BalanceTree<DAT, Idx>::CheckAll(){
    cout << "----------------------------------------" << endl;
    Node<DAT, Idx> *node = __Root__;
    int layer = 1;
    cout << "BTree ID : " << __ID__ << endl;
    while(node != NULL){
        cout << "Layer " << layer++ << " : ";
        Node<DAT, Idx> *temp = node;
        while(temp != NULL){
            cout << "[ ";
            for(int i = 0; i < temp->__cursor; i++){
                cout << temp->__index[i] << ",";
            }
            if(temp->__parent != NULL) cout << "(P:" << temp->__parent->__index[0] << ")";
            else cout << "(ROOT)";
            cout << " ]";
            cout << "->";
            temp = temp->__right;
        }
        cout << "<end>" << endl;
        if(node->__type == __LEAF__) break;
        node = node->__child[0];
    }
    cout << "----------------------------------------\n" << endl;
}

template<class DAT, class Idx>
void BalanceTree<DAT, Idx>::CutDown(){
    Node<DAT, Idx> *node = __Root__;
    while(node != NULL){
        Node<DAT, Idx> *temp = node;
        while(temp != NULL){
            if(temp->__type != __LEAF__){
                for(int i = 0; i < temp->__cursor; i++){
                    temp->__child[i + 1] = NULL;
                }
                if(temp != node){
                    temp->__child[0] = NULL;
                }
            }
            temp->__parent = NULL;
            delete[] temp->__index;
            delete[] temp->__data;
            if(temp != node){
                delete[] temp->__child;
            }
            temp = temp->__right;
            if(temp != NULL) temp->__left->__right = NULL;
        }
        if(node->__type == __LEAF__) break;
        temp = node;
        node = node->__child[0];
        delete[] temp->__child;
    }
    __Root__ = NULL;
    __Data__ = NULL;
}


/////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Node类基本函数
 *
 */

template<class DAT, class Idx>
Node<DAT, Idx>::Node(__NodeType__ type){
    /**
     * @brief 节点类构造函数 <parm>节点类型
     */
    __type = type;
    __parent = __left = __right = NULL;
    __index = new Idx [__ORDER__];
    //判断节点类型进行内存分配
    __data = (type == __LEAF__ ? new DAT [__ORDER__] : NULL);
    __child = (type == __INTERNAL__ ? new Node<DAT, Idx>*[__ORDER__ + 1] : NULL);
    __cursor = 0;
}

template<class DAT, class Idx>
bool Node<DAT, Idx>::isFull(){
    /**
     * @brief 判断节点是否已满
     */
    return __cursor >= __ORDER__;
}

template<class DAT, class Idx>
bool Node<DAT, Idx>::isSatisfied(){
    /**
     * @brief 判断节点关键字数量是否满足阶数要求
     */

    return __cursor - 1 >= __ORDER__ / 2 - 1;
}

template<class DAT, class Idx>
bool Node<DAT, Idx>::isLend(){
    /**
     * @brief 判断节点关键字数量是否满足可借要求
     */
    return __cursor - 1 > __ORDER__ / 2 - 1;
}

template<class DAT, class Idx>
bool Node<DAT, Idx>::isMerge(Node<DAT, Idx> *node){

    if(__cursor - 1 + node->__cursor - 1 < __ORDER__ &&
        node->__parent == __parent
        ) return true;
    return false;
}

template<class DAT, class Idx>
Node<DAT, Idx> *Node<DAT, Idx>::divide(){
    /**
     * @brief 根据参数位置分裂节点 <parm>分裂位置
     */
    Node<DAT, Idx> *new_node = NULL;
    if(this->__type == __LEAF__){
        new_node = new Node<DAT, Idx>(__LEAF__); //新增叶节点
        //数据转移
        for(int i = __ORDER__ / 2; i < __cursor; i++){
            new_node->insert(__index[i], __data[i]);
        }
    }
    else if(this->__type == __INTERNAL__){
        new_node = new Node<DAT, Idx>(__INTERNAL__); //新增内部节点
        //数据转移(提取中间索引)
        //最左侧节点指针初始化
        new_node->__child[0] = __child[__ORDER__ / 2 + 1];
        new_node->__child[0]->__parent = new_node;
        __child[__ORDER__ / 2 + 1] = NULL;
        //节点/孩子节点指针插入
        for(int i = __ORDER__ / 2 + 1; i < __cursor; i++){
            new_node->insert(__index[i], __child[i + 1]);
            __child[i + 1] = NULL;
        }
    }
    //调整节点间指针
    new_node->__right = __right;
    new_node->__left = this;
    if(__right != NULL)   __right->__left = new_node;
    __right = new_node;
    __cursor = __ORDER__ / 2;   //更新原节点光标
    return new_node;
}


template<class DAT, class Idx>
void Node<DAT, Idx>::remove(){
    /**
     * @brief 移除节点
     */

    if(__type == __INTERNAL__){
        for(int i = 0; i < __ORDER__ + 1; i++)    __child[i] = NULL;
    }
    if(__left != NULL) __left->__right = this->__right;
    __parent = __left = __right = NULL;
}

template<class DAT, class Idx>
void Node<DAT, Idx>::print_node(){
    /**
     * @brief 打印节点索引信息
     */
    for(int i = 0; i < __cursor; i++){
        cout << *__index[i] << " ";
    }cout << endl;
}
template<class DAT, class Idx>
Node<DAT, Idx> *Node<DAT, Idx>::get_side(int side){
    if(side == 0){
        return (__left == NULL ? NULL : __left);
    }
    else{
        return (__right == NULL ? NULL : __right);
    }
}

//////////////////////////////////////////////////////////////////////////////////
/**
 * @brief 数据查找相关函数
 *
 */

template<class DAT, class Idx>
Node<DAT, Idx> *BalanceTree<DAT, Idx>::find_insert_leaf_node(Idx& idx, Node<DAT, Idx> *start){
    /**
     * @brief 根据索引定位数据查找/插入的叶节点 <parm>索引指针, 起始节点指针
     */
    if(start == NULL) return NULL;
    if(start->__type == __LEAF__) return start;
    int p = start->find_insert_position(idx);
    if(p == -1) return NULL;
    return find_insert_leaf_node(idx, start->__child[p]);
}

template<class DAT, class Idx>
DataNode<DAT, Idx> BalanceTree<DAT, Idx>::LocateData(Idx& idx){
    /**
     * @brief 根据索引查找数据单元位置 <parm>索引指针
     */
    DataNode<DAT, Idx> data_node = * new DataNode<DAT, Idx>();
    Node<DAT, Idx> *node = find_insert_leaf_node(idx, __Root__);
    if(node == NULL) return data_node;
    if(idx < node->__index[0]){
        return data_node; //需要新建数据页
    }
    int p = 0;
    for(int i = 1; i < node->__cursor; i ++){
        if(node->__index[i - 1] <= idx && idx < node->__index[i]){
            p = i - 1;
            break;
        }
        p = node->__cursor - 1;
    }
    data_node.node = node;
    data_node.pos = p;
    return data_node;
}
/*
template<class DAT, class Idx>
DAT *BalanceTree<DAT, Idx>::AccurateLoacte(Idx *idx){
    // @brief 根据索引定位数据页 <parm>索引指针
    Node<DAT, Idx> *node = find_insert_leaf_node(idx, __Root__);
    if(node == NULL) return NULL;
    for(int i = 0; i < node->__cursor; i ++){
        if(*node->__index[i] == *idx){
            return node->__data[i];
        }
    }
    return NULL;
}*/

template<class DAT, class Idx>
int Node<DAT, Idx>::find_insert_position(Idx& idx){
    /**
     * @brief 根据索引查找位置 <parm>索引指针
     */
    if(isFull()) return -1;
    if(__cursor == 0 || idx < __index[0]) return 0;
    if(idx >= __index[__cursor - 1]) return __cursor;
    for(int i = 0; i < __cursor - 1; i ++){
        if(__index[i] <= idx && idx < __index[i + 1]){
            return i + 1;
        }
    }
    return -1;
}

template<class DAT, class Idx>
int Node<DAT, Idx>::find_delete_position(Idx& idx){
    for(int i = 0; i < __cursor; i ++){
        if(idx == __index[i]){
            return i;
        }
    }
    return -1;
}

template<class DAT, class Idx>
bool Node<DAT, Idx>::find_node(Node<DAT, Idx> *node){
    if(this == node) return true;
    if(__type == __LEAF__) return false;
    for(int i = 0; i <= __cursor; i++){
        if(__child[i]->find_node(node)){
            return true;
        }
    }
    return false;
}


template<class DAT, class Idx>
Idx* BalanceTree<DAT, Idx>::common_index_locate(Node<DAT, Idx> *A, Node<DAT, Idx> *B, Node<DAT, Idx> *start){
    if(start->__type == __LEAF__) return NULL;
    int A_pos = -1, B_pos = -1;
    for(int i = 0; i <= start->__cursor; i++){
        if(start->__child[i]->find_node(A)) A_pos = i;
        if(start->__child[i]->find_node(B)) B_pos = i;
    }
    if(A_pos == -1 || B_pos == -1) return NULL;
    if(B_pos - A_pos == 1) return &start->__index[A_pos];
    if(A_pos == B_pos) return common_index_locate(A, B, start->__child[B_pos]);
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief 数据插入相关函数
 *
 */

template<class DAT, class Idx>
void BalanceTree<DAT, Idx>::InsertData(Idx& idx, DAT& data){
    /**
     * @brief 插入数据单元 <parm>数据指针, 索引指针
     */
     //根节点为空, 创建根节点
    if(__Root__ == NULL){
        __Root__ = new Node<DAT, Idx>(__LEAF__);
        __Root__->insert(idx, data);
        __Data__ = __Root__;
        return;
    }
    Node<DAT, Idx> *node = find_insert_leaf_node(idx, __Root__);
    node->insert(idx, data);
    insert_adjust(node);
}

template<class DAT, class Idx>
void BalanceTree<DAT, Idx>::insert_adjust(Node<DAT, Idx> *node){
    /**
     * @brief 树结构调整 <parm>需调整节点指针
     */
    if(node == NULL || ! node->isFull()) return;
    Node<DAT, Idx> *resource = node->__parent;   //定位父亲节点
    Idx new_idx = node->__index[__ORDER__ / 2];  //确定新索引
    //节点分裂
    Node<DAT, Idx> *new_node = node->divide();
    //父节点操作
    if(resource == NULL){
        resource = new Node<DAT, Idx>(__INTERNAL__); //新建父节点索引
        resource->__child[0] = node;
        node->__parent = resource;
    }
    resource->insert(new_idx, new_node);
    __Root__ = (node == __Root__ ? resource : __Root__);  //更新根节点指针
    //向上递归调整树结构
    insert_adjust(resource);
}

template<class DAT, class Idx>
void Node<DAT, Idx>::insert(Idx& idx, DAT data){
    /**
     * @brief 插入数据指针 <parm>索引指针, 数据指针
     */
    if(isFull()) return;
    //找到需要插入的位置
    int insert_position = find_insert_position(idx);
    //原数据/索引后移
    for(int i = __cursor; i > insert_position; i--){
        __index[i] = __index[i - 1];
        __data[i] = __data[i - 1];
    }
    //插入数据/索引
    __data[insert_position] = data;
    __index[insert_position] = idx;
    __cursor ++;
}

template<class DAT, class Idx>
void Node<DAT, Idx>::insert(Idx& idx, Node<DAT, Idx> *node){
    /**
     * @brief 插入孩子节点指针 <parm>索引指针, 节点指针
     */
    if(isFull()) return;
    int insert_position = find_insert_position(idx);
    for(int i = __cursor; i > insert_position; i--){
        __index[i] = __index[i - 1];
        __child[i + 1] = __child[i];
    }
    __index[insert_position] = idx;
    __child[insert_position + 1] = node;
    node->__parent = this;  //更新父节点指针
    __cursor ++;   //更新光标
}

//////////////////////////////////////////////////////////////////////////////
/**
 * @brief 数据删除相关函数
 *
 */

template<class DAT, class Idx>
void BalanceTree<DAT, Idx>::DeleteData(Idx& idx){
    Node<DAT, Idx> *node = find_insert_leaf_node(idx, __Root__);
    if(node == NULL) return;
    if(!node->DeleteData(idx)) return;
    if(node == __Root__) return;
    if(node->isSatisfied()) return;
    //合并节点优先///////////////////////////////////////////////////////////////////
    if(node->__left != NULL && node->isMerge(node->__left)){
        //左合并
        Node<DAT, Idx> *temp = node;
        node = node->left_merge(node->__left);
        temp->__cursor = 0;
        if(node->__parent == __Root__ && node->__parent->__cursor == 0){
            __Root__ = node;
            __Root__->__parent = NULL;
            return;
        }
        delete_adjust(node->__parent);
    }
    else if(node->__right != NULL && node->isMerge(node->__right)){
        //右合并
        node->right_merge(node->__right);
        if(node->__parent == __Root__ && node->__parent->__cursor == 0){
            __Root__ = node;
            __Root__->__parent = NULL;
            return;
        }
        delete_adjust(node->__parent);
    }
    else if(node->__left != NULL && node->__left->isLend()){ //左节点可借
        //cout << "l lend" << endl;
        node->left_lend(idx);
        Idx *common_idx = common_index_locate(node->__left, node, __Root__); //找到共同父索引
        *common_idx = node->__index[0];    //更新共同父索引
    }
    else if(node->__right != NULL && node->__right->isLend()){ //右节点可借
        //cout << "r lend" << endl;
        node->right_lend(idx);
        Idx *common_idx = common_index_locate(node, node->__right, __Root__); //找到共同父索引
        *common_idx = node->__right->__index[0]; //更新共同父索引
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

template<class DAT, class Idx>
bool Node<DAT, Idx>::DeleteData(Idx& idx){
    /**
     * @brief 删除索引指向的数据指针 <parm>索引指针
     */
    int p = find_delete_position(idx);
    if(p == -1) return false;
    for(int i = p; i < __cursor - 1; i++){
        __data[i] = __data[i + 1];
        __index[i] = __index[i + 1];
    }
    __cursor --;
    return true;
}

template<class DAT, class Idx>
void Node<DAT, Idx>::delete_index(int p){
    /**
     * @brief 合并节点后删除父节点索引 <parm>删除索引位置
     */
    if(p == -1) return;
    for(int i = p; i < __cursor - 1; i++){
        __index[i] = __index[i + 1];
        __child[i + 1] = __child[i + 2];
    }
    __child[__cursor] = NULL;
    __cursor --;
}


template<class DAT, class Idx>
void BalanceTree<DAT, Idx>::delete_adjust(Node<DAT, Idx> *node){
    /**
     * @brief 删除后树结构调整 <parm>起始节点指针
     *
     */
    if(node == __Root__) return;    //根节点无需调整
    if(node->isSatisfied()) return; //满足树结构无需调整
    if(node->__left != NULL && node->__left->isLend()){ //左节点可借
        //拷贝原索引
        Idx temp_idx = node->__index[0];
        Idx *common_idx = common_index_locate(node->__left, node, __Root__); //找到共同父索引
        node->left_lend(*common_idx);   //当前节点索引更新
        *common_idx = temp_idx;    //更新共同父索引
        return;

    }
    else if(node->__right != NULL && node->__right->isLend()){ //右节点可借
       //拷贝原索引
        Idx temp_idx = node->__right->__index[0];
        Idx *common_idx = common_index_locate(node, node->__right, __Root__); //找到共同父索引
        node->right_lend(*common_idx);  //当前节点索引更新
        *common_idx = temp_idx;    //更新共同父索引
        return;
    }
    else{
        //拉取父索引
        int p;
        if(node->__left != NULL && node->__left->__parent == node->__parent){
            //左合并
            //定位父索引
            p = node->__parent->find_insert_position(node->__left->__index[node->__left->__cursor - 1]);
            Idx temp_idx = node->__parent->__index[p];   //拉取父索引
            Node<DAT, Idx> *temp = node;
            node = node_merge(node->__left, temp_idx, node);  //合并
            temp->__cursor = 0;
            node->__parent->delete_index(p);    //删除父索引
        }
        else if(node->__right != NULL && node->__right->__parent == node->__parent){
            //右合并
            p = node->__parent->find_insert_position(node->__index[node->__cursor - 1]);  //定位父索引
            Idx temp_idx = node->__parent->__index[p];   //拉取父索引
            node = node_merge(node, temp_idx, node->__right); //合并
            node->__parent->delete_index(p);    //删除父索引
        }
        if(node->__parent == __Root__ && __Root__->__cursor == 0){
            __Root__ = node;    //更新根节点
            node->__parent = NULL;
            return;
        }
    }
    if(node->__parent == __Root__) return;
    delete_adjust(node->__parent);  //递归调整判断
}

///////////////////////


template<class DAT, class Idx>
Node<DAT, Idx> *BalanceTree<DAT, Idx>::node_merge(Node<DAT, Idx> *nodeA, Idx& idx, Node<DAT, Idx> *nodeB){
    /**
     * @brief 拉取父节点索引后合并 <parm>左节点指针, 父索引, 右节点指针
     */
    nodeA->__index[nodeA->__cursor++] = idx;  //加入父索引
    //节点数据迁移
    for(int i = 0; i < nodeB->__cursor; i++){
        nodeA->__index[nodeA->__cursor] = nodeB->__index[i];
        nodeB->__child[i]->__parent = nodeA;
        nodeA->__child[nodeA->__cursor] = nodeB->__child[i];
        nodeB->__child[i] = NULL;
        nodeA->__cursor++;
    }
    nodeA->__child[nodeA->__cursor] = nodeB->__child[nodeB->__cursor];
    nodeB->__child[nodeB->__cursor] = NULL;
    nodeA->__child[nodeA->__cursor]->__parent = nodeA;
    //节点间指针更新
    nodeA->__right = nodeB->__right;
    if(nodeB->__right != NULL)    nodeB->__right->__left = nodeA;
    nodeB->remove();
    return nodeA;
}



template<class DAT, class Idx>
Node<DAT, Idx> *Node<DAT, Idx>::left_merge(Node<DAT, Idx> *node){
    /**
     * @brief 左节点合并 <parm>左节点指针
     */
    int p = __parent->find_insert_position(__index[0]);
    for(int i = 0; i < __cursor; i++){
        node->insert(__index[i], __data[i]);
    }
    node->__right = __right;
    if(node->__left != NULL) node->__left->__right = node;
    __left = __right = __parent = NULL;
    node->__parent->delete_index(p);
    return node;
}

template<class DAT, class Idx>
void Node<DAT, Idx>::right_merge(Node<DAT, Idx> *node){
    /**
     * @brief 右节点合并 <parm>右节点指针
     */
    int p = node->__parent->find_insert_position(node->__index[0]);
    for(int i = 0; i < node->__cursor; i++){
        this->insert(node->__index[i], node->__data[i]);
    }
    if(node->__right != NULL) node->__right->__left = this;
    this->__right = node->__right;
    node->__left = node->__right = node->__parent = NULL;
    __parent->delete_index(p-1);
}

template<class DAT, class Idx>
void Node<DAT, Idx>::left_lend(Idx& new_idx){
    /**
     * @brief 向左节点借数据 <parm> 本节点更新后的索引
     */
    Node<DAT, Idx> *left = this->__left;
    if(__type == __INTERNAL__){
        for(int i = __cursor; i > 0; i--){
            __index[i] = __index[i - 1];
            __child[i + 1] = __child[i];
        }__child[1] = __child[0];
        __child[0] = left->__child[left->__cursor];
        __child[0]->__parent = this;
        __index[0] = new_idx;
    }
    else if(__type == __LEAF__){
        for(int i = __cursor; i > 0; i--){
            __index[i] = __index[i - 1];
            __data[i] = __data[i - 1];
        }
        __data[0] = left->__data[left->__cursor - 1];
        __index[0] = left->__index[left->__cursor - 1];
    }
    left->__cursor --;
    __cursor ++;

}

template<class DAT, class Idx>
void Node<DAT, Idx>::right_lend(Idx& new_idx){
    /**
     * @brief 向右节点借数据 <parm> 本节点更新后的索引
     */
    Node<DAT, Idx> *right = this->__right;
    if(__type == __INTERNAL__){
        __child[__cursor + 1] = right->__child[0];
        right->__child[0]->__parent = this;
        for(int i = 0; i < right->__cursor; i++){
            if(i < right->__cursor - 1){
                right->__index[i] = right->__index[i + 1];
            }
            right->__child[i] = right->__child[i + 1];
        }
        right->__child[right->__cursor] = NULL;
        __index[__cursor] = new_idx;

    }
    else if(__type == __LEAF__){
        __data[__cursor] = right->__data[0];
        __index[__cursor] = right->__index[0];
        for(int i = 0; i < right->__cursor - 1; i++){
            right->__index[i] = right->__index[i + 1];
            right->__data[i] = right->__data[i + 1];
        }

    }
    right->__cursor --;
    __cursor ++;
}

/////////////////////////////////////////////////////////////////////////
//iterator simulation

template<class DAT, class Idx>
DataNode<DAT, Idx>::DataNode(){
    node = NULL;
    pos = -1;
}

template<class DAT, class Idx>
DataNode<DAT, Idx> &DataNode<DAT, Idx>::operator++(){
    if(pos < node->__cursor - 1){
        pos++;
    }
    else{
        if(node->__right != NULL){
            node = node->__right;
            pos = 0;
        }
        else{
            node = NULL;
        }
    }
    return *this;
}

template<class DAT, class Idx>
DataNode<DAT, Idx> &DataNode<DAT, Idx>::operator--(){
    if(pos > 0){
        pos--;
    }
    else{
        if(node->__left != NULL){
            node = node->__left;
            pos = node->__cursor - 1;
        }
        else{
            node = NULL;
        }
    }
    return *this;
}

template<class DAT, class Idx>
DAT* DataNode<DAT, Idx>::getData(){
    if(node == NULL || pos == -1 || pos >= node->__cursor){
        return NULL;
    }
    return &node->__data[pos];
}

template<class DAT, class Idx>
DataNode<DAT, Idx> BalanceTree<DAT, Idx>::getLink(){
    DataNode<DAT, Idx> node = * new DataNode<DAT, Idx>();
    node.node = findHead(__Root__);
    node.pos = 0;
    return node;
}
///////////////////////////////////////////////////////////////////////////////////////

#endif
