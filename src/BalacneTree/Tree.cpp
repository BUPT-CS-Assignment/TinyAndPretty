#include "BalanceTree.h"

template<class T>
BalanceTree<T>::BalanceTree(int id){
    __ID__ = id;
    __Data__ = NULL;
    __Root__ = NULL;
}

template<class T>
Node<T>* BalanceTree<T>::locate_node(Index* index,Node<T>* start){
    if(start == NULL) return NULL;
    if(start->__type == __LEAF__) return start;
    int p = start->find_position(index);
    if(p == -1) return NULL;
    return locate_node(index,start->__child[p]);
}


template<class T>
T* BalanceTree<T>::search_position(Index* index){
    Node<T>* node = locate_node(index,__Root__);
    if(node == NULL) return NULL;
    int p = node->find_position(index);
    if(p == -1) return NULL;
    return node->__keys[p];
}

template<class T>
void BalanceTree<T>::insert_data(T* t,Index* index){
    cout<<"input:"<<*index<<endl;
    if(__Root__ == NULL){
        __Root__ = new Node<T> ( __LEAF__ );
        __Root__ -> insert(t,index,NULL);
        return;
    }
    Node<T>* node = locate_node(index,__Root__);
    cout<<"L:";node->print_node();
    node->insert(t,index,NULL);
    insert_adjust(node);
    //__Root__->print_node();
}

template<class T>
void BalanceTree<T>::insert_adjust(Node<T>* node){
    if(node == NULL || !node->isFull()) return;
    Node<T>* resource = node->__parent;
    Index* new_index = new Index(node->__index[__ORDER__/2]);
    Node<T>* new_node = NULL;
    node->print_node();
    //节点分裂
    if(node->__type == __LEAF__){
        new_node = new Node<T>(__LEAF__);
        for(int i = __ORDER__/2; i < __ORDER__; i++){
            new_node->insert(node->__keys[i],node->__index[i],NULL);
            node->__keys[i] = NULL;
            node->__index[i]->~Index();
        }
        node->__right = new_node;
    }else{
        new_node = new Node<T>(__INTERNAL__);
        for(int i = __ORDER__/2 + 1; i < __ORDER__; i++){
            new_node->insert(NULL,node->__index[i],NULL);
            node->__index[i]->~Index();
        }
        for(int i = __ORDER__/2 + 1; i <= __ORDER__; i++){
             node->__child[i]->__parent = new_node;
             new_node->__child[i - __ORDER__/2 -1] = node->__child[i];
             node->__child[i] = NULL;
        }
    }
    node->__cursor = __ORDER__/2;
    if(resource == NULL){
        resource = new Node<T>(__INTERNAL__);
        resource->insert(NULL,NULL,node);  
    }
    resource->insert(NULL,new_index,new_node);
    __Root__ = (node == __Root__?resource : __Root__); 
    node->__parent = resource;
    new_node->__parent = resource;
    insert_adjust(resource);
}

template<class T>
void BalanceTree<T>::print_tree(){
    __Root__->print_node(__Root__);
}

    

