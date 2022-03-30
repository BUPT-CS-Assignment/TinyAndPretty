#include<Data.h>
#include<Implement.h>

Index::Index(){
    type_ = __TEXT;
}

Index::Index(DATA_TYPE type){
    type_ = type;
}

int Index::getSize(){
    switch(type_){
        case __INT: return 4;
        case __INT64: case __REAL:
            return 8;
        case __TEXT: return 32;
        case __LONGTEXT: return 255;
        default: return 0;
    }
}

Index::Index(string index, DATA_TYPE type){
    if(type == __INT && parm_check(index, __INT)){
        index_.i_index = stoi(index);
    }
    else if(type == __INT64 && parm_check(index, __INT64)){
        index_.l_index = stoll(index);
    }
    else if(type == __REAL && parm_check(index, __REAL)){
        index_.d_index = stod(index);
    }
    else if(type == __TEXT && parm_check(index, __TEXT)){
        const char *str = index.c_str();
        strcpy(index_.t_index, str);
    }
    type_ = type;
}

Index::Index(int i){
    type_ = __INT;
    index_.i_index = i;
}

Index::Index(long long ll){
    type_ = __INT64;
    index_.l_index = ll;
}

Index::Index(double d){
    type_ = __REAL;
    index_.d_index = d;
}

Index::Index(string s){
    type_ = __TEXT;
    strcpy(index_.t_index, s.c_str());
}





ostream &operator << (ostream &out, Index &index){
    if(index.type_ == __INT) out << index.index_.i_index;
    else if(index.type_ == __INT64) out << index.index_.l_index;
    else if(index.type_ == __REAL) out << index.index_.d_index;
    else if(index.type_ == __TEXT) out << index.index_.t_index;
    return out;
}

bool Index::operator<(Index &index){
    if(type_ == __INT) return index_.i_index < index.index_.i_index;
    if(type_ == __INT64) return index_.l_index < index.index_.l_index;
    if(type_ == __REAL) return index_.d_index < index.index_.d_index;
    return strcmp(index_.t_index, index.index_.t_index) < 0;
}

bool Index::operator>(Index &index){
    if(type_ == __INT) return index_.i_index > index.index_.i_index;
    if(type_ == __INT64) return index_.l_index > index.index_.l_index;
    if(type_ == __REAL) return index_.d_index > index.index_.d_index;
    return strcmp(index_.t_index, index.index_.t_index) > 0;
}

bool Index::operator==(Index &index){
    if(type_ == __INT) return index_.i_index == index.index_.i_index;
    if(type_ == __INT64) return index_.l_index == index.index_.l_index;
    if(type_ == __REAL) return index_.d_index == index.index_.d_index;
    return strcmp(index_.t_index, index.index_.t_index) == 0;
}
bool Index::operator<=(Index &index){
    if(type_ == __INT) return index_.i_index <= index.index_.i_index;
    if(type_ == __INT64) return index_.l_index <= index.index_.l_index;
    if(type_ == __REAL) return index_.d_index <= index.index_.d_index;
    return strcmp(index_.t_index, index.index_.t_index) <= 0;
}
bool Index::operator>=(Index &index){
    if(type_ == __INT) return index_.i_index >= index.index_.i_index;
    if(type_ == __INT64) return index_.l_index >= index.index_.l_index;
    if(type_ == __REAL) return index_.d_index >= index.index_.d_index;
    return strcmp(index_.t_index, index.index_.t_index) >= 0;
}