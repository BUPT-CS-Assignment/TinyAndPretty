#include<Basic/data.h>
#include<Utils/implement.h>
using namespace std;

Index::Index(){
    type_ = __TEXT;
    memset(&index_, 0, sizeof(INDEX));
}

Index::~Index(){
    memset(&index_, 0, sizeof(INDEX));
}

int Index::getSize(){
    switch(type_){
        case __INT: return 4;
        case __INT64: case __REAL:
            return 8;
        case __TEXT: return 32;
        default: return 0;
    }
}

void Index::setVal(DATA_TYPE type, void* src){
    try{
        type_ = type;
        memset(&index_, 0, sizeof(INDEX));
        if(src == NULL) return;
        if(type == __INT){
            index_.i_index = (int)(*(int*)src);
        }
        else if(type == __INT64){
            index_.l_index = (long long)(*(long long*)src);
        }
        else if(type == __REAL){
            index_.d_index = (double)(*(double*)src);
        }
        else if(type == __TEXT){
            if(strlen((char*)src) > 31){
                throw TYPE_TEXT_OVERFLOW;
            }
            strcpy(index_.t_index, (char*)src);
        }
    }
    catch(NEexception& e){
        throw e;
    }

}

Index::Index(string index, DATA_TYPE type){
    try{
        memset(&index_, 0, sizeof(INDEX));
        if(type == __INT){
            parm_check(index, __INT);
            index_.i_index = stoi(index);
        }
        else if(type == __INT64){
            parm_check(index, __INT64);
            index_.l_index = stoll(index);
        }
        else if(type == __REAL){
            parm_check(index, __REAL);
            index_.d_index = stod(index);
        }
        else if(type == __TEXT){
            parm_check(index, __TEXT);
            const char* str = index.c_str();
            strcpy(index_.t_index, str);
        }
        type_ = type;
    }
    catch(NEexception& e){
        throw e;
    }
}

Index::Index(int i){
    type_ = __INT;
    memset(&index_, 0, sizeof(INDEX));
    index_.i_index = i;
}

Index::Index(long long ll){
    type_ = __INT64;
    memset(&index_, 0, sizeof(INDEX));
    index_.l_index = ll;
}

Index::Index(double d){
    type_ = __REAL;
    memset(&index_, 0, sizeof(INDEX));
    index_.d_index = d;
}

Index::Index(string s){
    type_ = __TEXT;
    memset(&index_, 0, sizeof(INDEX));
    strcpy(index_.t_index, s.c_str());
}

Index::Index(Index& src){
    memset(&index_, 0, sizeof(INDEX));
    memcpy(&index_, &src.index_, sizeof(INDEX));
    type_ = src.type_;
}

Index& Index::operator=(Index& src){
    memset(&index_, 0, sizeof(INDEX));
    memcpy(&index_, &src.index_, sizeof(INDEX));
    type_ = src.type_;
    return *this;
}

ostream& operator << (ostream& out, Index& index){
    if(index.type_ == __INT) out << index.index_.i_index;
    else if(index.type_ == __INT64) out << index.index_.l_index;
    else if(index.type_ == __REAL) out << index.index_.d_index;
    else if(index.type_ == __TEXT) out << index.index_.t_index;
    return out;
}

bool Index::operator<(Index& index){
    if(type_ == __INT) return index_.i_index < index.index_.i_index;
    if(type_ == __INT64) return index_.l_index < index.index_.l_index;
    if(type_ == __REAL) return index_.d_index < index.index_.d_index;
    return strcmp(index_.t_index, index.index_.t_index) < 0;
}

bool Index::operator>(Index& index){
    if(type_ == __INT) return index_.i_index > index.index_.i_index;
    if(type_ == __INT64) return index_.l_index > index.index_.l_index;
    if(type_ == __REAL) return index_.d_index > index.index_.d_index;
    return strcmp(index_.t_index, index.index_.t_index) > 0;
}

bool Index::operator==(Index& index){
    if(type_ == __INT) return index_.i_index == index.index_.i_index;
    if(type_ == __INT64) return index_.l_index == index.index_.l_index;
    if(type_ == __REAL) return index_.d_index == index.index_.d_index;
    return strcmp(index_.t_index, index.index_.t_index) == 0;
}
bool Index::operator<=(Index& index){
    if(type_ == __INT) return index_.i_index <= index.index_.i_index;
    if(type_ == __INT64) return index_.l_index <= index.index_.l_index;
    if(type_ == __REAL) return index_.d_index <= index.index_.d_index;
    return strcmp(index_.t_index, index.index_.t_index) <= 0;
}
bool Index::operator>=(Index& index){
    if(type_ == __INT) return index_.i_index >= index.index_.i_index;
    if(type_ == __INT64) return index_.l_index >= index.index_.l_index;
    if(type_ == __REAL) return index_.d_index >= index.index_.d_index;
    return strcmp(index_.t_index, index.index_.t_index) >= 0;
}