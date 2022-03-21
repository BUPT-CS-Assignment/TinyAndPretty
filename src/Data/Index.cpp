#include<Data.h>
#include<Implement.h>

Index::Index(){
    index_type = __TEXT;
}

Index::Index(string index,__DataType__ type){
    if(type == __INT && parm_check(index,__INT)){
        i_index = stoi(index);
    }else if(type == __INT64 && parm_check(index,__INT64)){
        l_index = stoll(index);
    }else if(type == __REAL && parm_check(index,__REAL)){
        d_index = stod(index);
    }else if(type == __TEXT && parm_check(index,__TEXT)){
        s_index = *(new string(index));
    }else if(type == __LONGTEXT && parm_check(index,__LONGTEXT)){
        s_index = *(new string(index));
    }
    index_type = type;
}

ostream & operator << (ostream & out, Index & index){
    if(index.index_type == __INT) out << index.i_index;
    else if(index.index_type == __INT64) out << index.l_index;
    else if(index.index_type == __REAL) out << index.d_index;
    else out << index.s_index;
    return out;
}

bool Index::operator<(Index& index){
    if(index_type == __INT) return i_index < index.i_index;
    if(index_type == __INT64) return l_index < index.l_index;
    if(index_type == __REAL) return d_index < index.d_index;
    return s_index < index.s_index;
}

bool Index::operator>(Index& index){
    if(index_type == __INT) return i_index > index.i_index;
    if(index_type == __INT64) return l_index > index.l_index;
    if(index_type == __REAL) return d_index > index.d_index;
    return s_index > index.s_index;
}

bool Index::operator==(Index& index){
    if(index_type == __INT) return i_index == index.i_index;
    if(index_type == __INT64) return l_index == index.l_index;
    if(index_type == __REAL) return d_index == index.d_index;
    return s_index == index.s_index;
}
bool Index::operator<=(Index& index){
    if(index_type == __INT) return i_index <= index.i_index;
    if(index_type == __INT64) return l_index <= index.l_index;
    if(index_type == __REAL) return d_index <= index.d_index;
    return s_index <= index.s_index;
}
bool Index::operator>=(Index& index){
    if(index_type == __INT) return i_index >= index.i_index;
    if(index_type == __INT64) return l_index >= index.l_index;
    if(index_type == __REAL) return d_index >= index.d_index;
    return s_index >= index.s_index;
}