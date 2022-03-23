#include <Data.h>
#include <Implement.h>


Row::Row(Table* t){
    /*
        根据表元素格式初始化行，分配内存空间
    */
    this->t = t;
    __RowContent = new void*[t->__ParmNum];
    for(int i = 0; i < t->__ParmNum; i ++){
        switch(t->__ParmType[i]){
            case __INT :
                __RowContent[i] = new int;   
                break;
            case __INT64 :
                __RowContent[i] = new long long;   
                break;
            case __REAL :
                __RowContent[i] = new double;   
                break;
            case __TEXT :
                __RowContent[i] = new char[TEXT_LENGTH];   
                memset(__RowContent[i],'\0',TEXT_LENGTH); 
                break;
            case __LONGTEXT :
                __RowContent[i] = new char[LONGTEXT_LENGTH];
                memset(__RowContent[i],'\0',LONGTEXT_LENGTH);   
                break;
            default:
                __RowContent[i] = NULL;
                break;
        }
    }
}
    
bool Row::padding(string statement){
    /*
        填充行元素
        Format  @  '(element_1,_element_2,element_3,...)'
    */
    int l = 0;
    string* words = Split(statement,',',l); 
    //判断元素匹配
    if(l != t->__ParmNum){
        cout<<t->__ParmNum<<endl;
        cout<<"Parameter Number Mismatch."<<endl;   
        return false;
    }
    //写入内存
    for(int i = 0; i < t->__ParmNum; i ++){
        __DataType__ type = t->__ParmType[i];
        if(type == t->getKeyType()){
            __RowIndex = *(new Index(words[i],type));
        } 
        if(!parm_check(words[i],type)) return false;
        if(type == __INT){
            *((int*)__RowContent[i]) = stoi(words[i]);
        }else if(type == __INT64){
            *((long long*)__RowContent[i]) = stoll(words[i]);
        }else if(type == __REAL){
            *((double*)__RowContent[i]) = stod(words[i]);
        }else{
            strcpy((char*)__RowContent[i],words[i].c_str());
        }
    }
    return true;
}  

char* Row::format(){
    /*
        格式化字符串
        Format  @  'element_1,_element_2,element_3,...'
    
    */
    string temp = "";
    for(int i = 0; i < t->__ParmNum ;i++){
        switch(t->__ParmType[i]){
            case __INT :
                temp = temp + to_string(*((int*)__RowContent[i]));
                break;
            case __INT64 :
                temp = temp + to_string(*((long long*)__RowContent[i]));
                break;
            case __REAL :
                temp = temp + to_string(*((double*)__RowContent[i]));
                break;
            default:
                temp = temp + (char*)__RowContent[i];
                break;
        }
        temp = temp + (i== t->__ParmNum-1?"\n":"\t ");
    }
    char* row = new char[temp.length()+1];
    strcpy(row,temp.c_str());
    return row;
}

void Row::erase(){
    /*
        删除行
    */
    for(int i = 0;i<t->__ParmNum;i++){
        switch(t->__ParmType[i]){
            case __INT :
                delete[] (int*)__RowContent[i];
                break;
            case __INT64 :
                delete[] (long long*)__RowContent[i];
                break;
            case __REAL :
                delete[] (double*)__RowContent[i];
                break;
            default : 
                delete[] (char*)__RowContent[i];
                break;
        }
    }
}

void Row::index_update(){
    __DataType__ type = t->__ParmType[t->__Key];
    __RowIndex = *new Index(type);
    switch(type){
        case __INT :
            __RowIndex.INDEX.i_index = *((int*)__RowContent[t->__Key]); break;
        case __INT64 :
            __RowIndex.INDEX.l_index = *((long long*)__RowContent[t->__Key]); break;
        case __REAL :
            __RowIndex.INDEX.d_index = *((double*)__RowContent[t->__Key]); break;
        case __TEXT :
            strcpy(__RowIndex.INDEX.t_index,(char*)__RowContent[t->__Key]); break;
        default:
            break;
    }

}

Index& Row::getIndex(){
    
    return __RowIndex;
}

bool Row::operator<(Row& row){
    return __RowIndex < row.__RowIndex;
}
bool Row::operator>(Row& row){
    return __RowIndex > row.__RowIndex;
}
bool Row::operator==(Row& row){
    return __RowIndex == row.__RowIndex;
}
bool Row::operator<=(Row& row){
    return __RowIndex <= row.__RowIndex;
}
bool Row::operator>=(Row& row){
    return __RowIndex >= row.__RowIndex;
}