#include "BalanceTree.h"

template<class T>
Node<T>::Node(__NodeType__ type){
    __type = type;
    __parent = __right = NULL;
    __index = new Index*[__ORDER__ ];
    __keys = (type == __LEAF__? new T*[__ORDER__ ] : NULL);
    __child = (type == __INTERNAL__? new Node<T>*[__ORDER__ + 1] : NULL);
    __cursor = 0;
}

template<class T>
bool Node<T>::isFull(){
    return __cursor >= __ORDER__; 
}

template<class T>
void Node<T>::insert(T* t,Index* index,Node<T>* node){
    if(isFull()) return;
    //找到需要插入的位置
    if(index == NULL){
        __child[0] = node;
        return;
    }
    int p = 0;
    if(__cursor == 0 || *index < *__index[0]){
        p = 0;
    }else if(*index > *__index[__cursor -1]){
        p = __cursor;
    }else{
        for(int i = 0;i < __cursor -1; i++){
            if(*index > *__index[i] && *index < *__index[i+1] ){
                p = i+1;
                break;
            }
        }
    }
    //移动原节点位置
    if(__type == __INTERNAL__){
        for(int i = __cursor; i > p; i--){
            __index[i] = __index[i-1];
            __child[i+1] = __child[i];
        }
        __child[p+1] = node;
    }else{
        for(int i = __cursor; i > p; i--){
            __index[i] = __index[i-1];
            __keys[i] = __keys[i-1];
        }
        __keys[p] = t;
    }
    //插入信索引
    __index[p] = new Index(index);
    __cursor ++;
}

template<class T>
void Node<T>::remove(){
    if(__type == __INTERNAL__){
        for(int i = 0; i<= __cursor; i++){
            __child[i] = NULL;
        }
    }else{
        for(int i = 0; i < __cursor; i++){
            __keys[i] = NULL;
        }
    }
    delete[] __keys;
    delete[] __child;
    delete[] __index;
    __parent = __right = NULL;   
}

template<class T>
int Node<T>::find_position(Index* index){
    if(__cursor == 0) return -1;
    if(*__index[0] > *index) return 0;
    if(*__index[__cursor-1] < *index) return __cursor;
    for(int i = 1; i < __cursor; i ++){
        if(*__index[i-1] < *index && *__index[i] > *index){
            return i;
        }
    }
    return -1;
}

template<class T>
void Node<T>::print_node(){
    for(int i = 0;i < __cursor; i++){
        cout<<*(__index[i])<<" ";
    }cout<<endl;

    
}



