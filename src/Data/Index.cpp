#include<Data.h>
#include<Implement.h>

Index::Index(){
    index_type = __TEXT;
}

Index::Index(__DataType__ type){
    index_type = type;
}

Index::Index(string index,__DataType__ type){
    if(type == __INT && parm_check(index,__INT)){
        INDEX.i_index = stoi(index);
    }else if(type == __INT64 && parm_check(index,__INT64)){
        INDEX.l_index = stoll(index);
    }else if(type == __REAL && parm_check(index,__REAL)){
        INDEX.d_index = stod(index);
    }else if(type == __TEXT && parm_check(index,__TEXT)){
        const char* str = index.c_str();
        strcpy(INDEX.t_index,str);
    }
    index_type = type;
}





ostream & operator << (ostream & out, Index & index){
    if(index.index_type == __INT) out << index.INDEX.i_index;
    else if(index.index_type == __INT64) out << index.INDEX.l_index;
    else if(index.index_type == __REAL) out << to_string(index.INDEX.d_index);
    else if(index.index_type == __TEXT) out << index.INDEX.t_index;
    return out;
}

bool Index::operator<(Index& index){
    if(index_type == __INT) return INDEX.i_index < index.INDEX.i_index;
    if(index_type == __INT64) return INDEX.l_index < index.INDEX.l_index;
    if(index_type == __REAL) return INDEX.d_index < index.INDEX.d_index;
    return strcmp(INDEX.t_index,index.INDEX.t_index)<0;
}

bool Index::operator>(Index& index){
    if(index_type == __INT) return INDEX.i_index > index.INDEX.i_index;
    if(index_type == __INT64) return INDEX.l_index > index.INDEX.l_index;
    if(index_type == __REAL) return INDEX.d_index > index.INDEX.d_index;
    return strcmp(INDEX.t_index,index.INDEX.t_index)>0;
}

bool Index::operator==(Index& index){
    if(index_type == __INT) return INDEX.i_index == index.INDEX.i_index;
    if(index_type == __INT64) return INDEX.l_index == index.INDEX.l_index;
    if(index_type == __REAL) return INDEX.d_index == index.INDEX.d_index;
    return strcmp(INDEX.t_index,index.INDEX.t_index)==0;
}
bool Index::operator<=(Index& index){
    if(index_type == __INT) return INDEX.i_index <= index.INDEX.i_index;
    if(index_type == __INT64) return INDEX.l_index <= index.INDEX.l_index;
    if(index_type == __REAL) return INDEX.d_index <= index.INDEX.d_index;
    return strcmp(INDEX.t_index,index.INDEX.t_index)<=0;
}
bool Index::operator>=(Index& index){
    if(index_type == __INT) return INDEX.i_index >= index.INDEX.i_index;
    if(index_type == __INT64) return INDEX.l_index >= index.INDEX.l_index;
    if(index_type == __REAL) return INDEX.d_index >= index.INDEX.d_index;
    return strcmp(INDEX.t_index,index.INDEX.t_index)>=0;
}