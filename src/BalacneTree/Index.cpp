#include "BalanceTree.h"


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
        return __ll < index.__ll;
    else 
        return __str->compare(*index.__str) < 0;

}


bool Index::operator == (Index& index){
    if(__str == NULL)
        return __ll == index.__ll;
    else 
        return __str->compare(*index.__str) == 0;
}


bool Index::operator>(Index& index){
    if(__str == NULL)
        return __ll > index.__ll;
    else 
        return __str->compare(*index.__str) > 0;
}


Index& Index::operator=(Index& index){
    if(this != &index){
        __ll = new long long(*index.__ll);
        __str = new string(*index.__str);
    }return *this;
}
