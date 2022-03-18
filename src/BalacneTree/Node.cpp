#include "BalanceTree.h"

template<class T>
Node<T>::Node(){
    __type = __INTERNAL;
    __parent = __left = __right = NULL;
    __keys = new Key[__ORDER__ - 1];
    __child = new Node*[__ORDER__];
    __cursor = 0;
}


template<class T>
bool Node<T>::isFull(){
    return __cursor >= __ORDER__; 
}

template<class T>
void Node<T>::insert(T* t,Index& index){
    if(isFull()) return;
    __keys[__cursor ++ ] = new Key(index,t);
}

template<class T>
void Node<T>::remove(){
    for(int i = 0;i<=__cursor ;i++){
        if(i<__cursor) delete[] __keys[i];
        __child[i] = NULL;
    }
    delete[] __child;
    __parent = __left = __right = NULL;
    
}



