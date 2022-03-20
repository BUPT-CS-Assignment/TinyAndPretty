#include<Data/Table.h>
#include<Data/Page.h>
#include<Data/Row.h>
#include <Implement.h>


Table::Row::Row(Table* t){
    /*
        根据表元素格式初始化行，分配内存空间
    */
   this->t = t;
    __Content = new void*[t->__Data_Num];
    for(int i = 0; i < t->__Data_Num; i ++){
        switch(t->__Data_Type[i]){
            case __Short :
                __Content[i] = new short;   
                break;
            case __Int :
                __Content[i] = new int;   
                break;
            case __UInt :
                __Content[i] = new unsigned int;   
                break;
            case __Char :
                __Content[i] = new char;   
                break;
            case __VarChar11 :
                __Content[i] = new char[12];   
                break;
            case __VarChar32 :
                __Content[i] = new char[33];   
                break;
            case __VarChar255 :
                __Content[i] = new char[256];   
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
        if(type == __Short){
            int number = 0;
            if(!OverflowCheck(words[i],__Short) || ! to_Int(words[i],number)) return false;
            *((int*)__Content[i]) = number;
        }else if(type == __Int){
            int number = 0;
            if(!OverflowCheck(words[i],__Int) || ! to_Int(words[i],number)) return false;
            *((int*)__Content[i]) = number;
        }else if(type == __UInt){
            int number = 0;
            if(!OverflowCheck(words[i],__UInt) || ! to_Int(words[i],number)) return false;
            *((int*)__Content[i]) = number;
        }else if(type == __Char){
            if(!OverflowCheck(words[i],__Char)) return false;
            char c = words[i][0];
            *(char*)__Content[i] = c;
            //char* temp = (char*)__Content[i];
            //strcpy(temp,c);
        }else if(type == __VarChar11){
            if(!OverflowCheck(words[i],__VarChar11)) return false;
            char* temp = (char*)__Content[i];
            strcpy(temp,words[i].c_str());
        }else if(type == __VarChar32){
            if(!OverflowCheck(words[i],__VarChar32)) return false;
            char* temp = (char*)__Content[i];
            strcpy(temp,words[i].c_str());
        }else{
            if(!OverflowCheck(words[i],__VarChar255)) return false;
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
            case __Short :
                temp = temp + to_string(*((short*)__Content[i]));
                break;
            case __Int :
                temp = temp + to_string(*((int*)__Content[i]));
                break;
            case __UInt :
                temp = temp + to_string(*((unsigned int*)__Content[i]));
                break;
            case __Char :
                temp = temp + *(char*)__Content[i];
                break;
            case __VarChar11 :
            case __VarChar32 :
            case __VarChar255 :
                temp = temp + (char*)__Content[i];
                break;
        }
        temp = temp + (i== t->__Data_Num-1?"\n":"\t");
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
            case __Short :
                delete[] (short*)__Content[i];
                break;
            case __Int :
                delete[] (int*)__Content[i];
                break;
            case __UInt :
                delete[] (unsigned int*)__Content[i];
                break;
            case __Char :
            case __VarChar11 :
            case __VarChar32 :
            case __VarChar255 :
                delete[] (char*)__Content[i];
                break;
        }
    }
}