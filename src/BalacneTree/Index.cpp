#include "BalanceTree.h"

Index::Index(){
    __ll = NULL;
    __str = NULL;
}


Index::Index(Index* index){
    if(index->__ll != NULL) __ll = new long long(*index->__ll);
    if(index->__str != NULL) __str = new string(*index->__str);
}

Index::Index(long long i){
    __ll = new long long (i);
    __str = NULL;
}

Index::Index(string str){
    __ll = NULL;
    __str = new string(str);
}


bool Index::operator<(Index& index){
    if(__str == NULL)
        return *__ll < *index.__ll;
    else 
        return __str->compare(*index.__str) < 0;

}


bool Index::operator == (Index& index){
    if(__str == NULL)
        return *__ll == *index.__ll;
    else 
        return __str->compare(*index.__str) == 0;
}


bool Index::operator>(Index& index){
    if(__str == NULL)
        return *__ll > *index.__ll;
    else 
        return __str->compare(*index.__str) > 0;
}

/*
Index* Index::operator=(Index* index){
    if(this != index){
        *__ll = *index->__ll;
        *__str = *index->__str;
    }return this;
}
*/
ostream& operator << (ostream& output,Index& index){
    if(index.__ll != NULL){
        output << *index.__ll;
    }else if(index.__str !=NULL){
        output << *index.__str;
    }else{
        output<< "<Index Empty>";
    }
    return output;
}

Index::~Index(){
    delete __ll;
    delete __str;
}
