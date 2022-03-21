#include <Data.h>
#include <Implement.h>


Table::Row::Row(Table* t){
    /*
        根据表元素格式初始化行，分配内存空间
    */
    this->t = t;
    __Content = new void*[t->__Data_Num];
    for(int i = 0; i < t->__Data_Num; i ++){
        switch(t->__Data_Type[i]){
            case __INT :
                __Content[i] = new int;   
                break;
            case __INT64 :
                __Content[i] = new long long;   
                break;
            case __REAL :
                __Content[i] = new double;   
                break;
            case __TEXT :
                __Content[i] = new char[TEXT_LENGTH + 1];   
                break;
            case __LONGTEXT :
                __Content[i] = new char[LONGTEXT_LENGTH + 1];   
                break;
            default:
                __Content[i] = NULL;
                break;
        }
    }
}
    
bool Table::Row::padding(string statement){
    /*
        填充行元素
        Format  @  '(element_1,_element_2,element_3,...)'
    */
    int l = 0;
    string* words = Split(statement,',',l); 
    //判断元素匹配
    if(l != t->__Data_Num){
        cout<<"Parameter Number Mismatch."<<endl;   
        return false;
    }
    //写入内存
    for(int i = 0; i < t->__Data_Num; i ++){
        __DataType__ type = t->__Data_Type[i];
        if(type == t->__Index_Type){
            __Index = *(new Index(words[i],type));
        } 
        if(!parm_check(words[i],type)) return false;
        if(type == __INT){
            *((int*)__Content[i]) = stoi(words[i]);
        }else if(type == __INT64){
            *((long long*)__Content[i]) = stoll(words[i]);
        }else if(type == __REAL){
            *((long long*)__Content[i]) = stod(words[i]);
        }else{
            char* temp = (char*)__Content[i];
            strcpy(temp,words[i].c_str());
        }
    }
    return true;
}  

char* Table::Row::format(){
    /*
        格式化字符串
        Format  @  'element_1,_element_2,element_3,...'
    
    */
    string temp = "";
    for(int i = 0; i < t->__Data_Num ;i++){
        switch(t->__Data_Type[i]){
            case __INT :
                temp = temp + to_string(*((int*)__Content[i]));
                break;
            case __INT64 :
                temp = temp + to_string(*((long long*)__Content[i]));
                break;
            case __REAL :
                temp = temp + to_string(*((double*)__Content[i]));
                break;
            default:
                temp = temp + (char*)__Content[i];
                break;
        }
        temp = temp + (i== t->__Data_Num-1?"\n":"\t ");
    }
    char* row = new char[temp.length()+1];
    strcpy(row,temp.c_str());
    return row;
}

void Table::Row::erase(){
    /*
        删除行
    */
    for(int i = 0;i<t->__Data_Num;i++){
        switch(t->__Data_Type[i]){
            case __INT :
                delete[] (int*)__Content[i];
                break;
            case __INT64 :
                delete[] (long long*)__Content[i];
                break;
            case __REAL :
                delete[] (double*)__Content[i];
                break;
            default : 
                delete[] (char*)__Content[i];
                break;
        }
    }
}

Index& Table::Row::getIndex(){
    return __Index;
}

bool Table::Row::operator<(Row& row){
    return __Index < row.__Index;
}
bool Table::Row::operator>(Row& row){
    return __Index > row.__Index;
}
bool Table::Row::operator==(Row& row){
    return __Index == row.__Index;
}
bool Table::Row::operator<=(Row& row){
    return __Index <= row.__Index;
}
bool Table::Row::operator>=(Row& row){
    return __Index >= row.__Index;
}