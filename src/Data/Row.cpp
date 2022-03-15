#include <Data.h>
#include <Implement.h>


Row::Row(Table table){
    /*
        根据表元素格式初始化行，分配内存空间
    */
    __DataType__* element_type = table.getElementType();
    int element_number = table.getElementNumber();
    content = new void*[element_number];
    for(int i = 0; i < element_number; i ++){
        switch(element_type[i]){
            case __VarChar32 :
                content[i] = new char[32];   
                break;
            case __VarChar255 :
                content[i] = new char[255];   
                break;
            case __Int :
                content[i] = new int;   
                break;
        }
    }
}
    
bool Row::Padding(Table table,string statement){
    /*
        填充行元素
        Format  @  '(element_1,_element_2,element_3,...)'
    */
    int l = 0;
    string* words = Split(statement,',',l); 
    int element_number = table.getElementNumber();
    __DataType__* element_type = table.getElementType();
    //判断元素匹配
    if(l != element_number){
        cout<<"Parameter Number Mismatch."<<endl;   
        return false;
    }
    //写入内存
    for(int i = 0; i < element_number; i ++){
        if(element_type[i] == __Int){
            int number = 0;
            if(! to_Int(words[i],number)) return false;
            *((int*)content[i]) = number;
        }else{
            char* temp = (char*)content[i];
            strcpy(temp,words[i].c_str());
        }
    }
    return true;
}  

char* Row::Format(Table table){
    /*
        格式化字符串
        Format  @  'element_1,_element_2,element_3,...'
    
    */
    int element_number = table.getElementNumber();
    __DataType__* element_type = table.getElementType();
    string temp = "";
    for(int i = 0;i<element_number;i++){
        switch(element_type[i]){
            case __Int :
                temp = temp + to_string(*((int*)content[i]));
                break;
            case __VarChar32 :
            case __VarChar255 :
                temp = temp + (char*)content[i];
                break;
        }
        temp = temp + (i==element_number-1?"\n":"\t");
    }
    char* row = new char[temp.length()+1];
    strcpy(row,temp.c_str());
    return row;
}

void Row::Erase(Table table){
    /*
        删除行
    */
    int element_number = table.getElementNumber();
    __DataType__* element_type = table.getElementType();
    for(int i = 0;i<element_number;i++){
        switch(element_type[i]){
            case __Int :
                delete[] (int*)content[i];
                break;
            case __VarChar32 :
            case __VarChar255 :
                delete[] (char*)content[i];
                break;
        }
    }
}