#include<Implement.h>


string *Split(string str, char c, int &length){
    if(str.length() == 0){
        length = 0;
        return NULL;
    }
    int count = 1;   // 拆分部分数量
    int index = 0;   // 字符索引位置
    int l = str.length();
    index = str.find(c, index);
    while(index < l && index >= 0){
        count++;
        index++;
        index = str.find(c, index);
    }
    string *words = new string[count];   //拆分部分数组
    int start = 0, end = str.find(c, start);
    for(int i = 0; i < count; i++){
        words[i] = Trim(str.substr(start, end - start));
        start = end + 1;
        end = str.find(c, start);
        if(end == -1) end = str.length();
    }
    length = count;
    return words;
}

string Trim(string str){
    string temp = str;
    temp.erase(0, temp.find_first_not_of(" "));
    temp.erase(temp.find_last_not_of(" ") + 1);
    return temp;
}

bool parm_check(string str, DATA_TYPE type){
    if(str.length() == 0){
        cout << "<E> PARAMETER EMPTY" << endl;
        return false;
    }
    if(type == __INT){
        if(!regex_match(str, regex("^-?\\d+"))){
            cout << "<E> PARAMETER '" << str << "' MISMATCH TYPE 'INT'." << endl;
            return false;
        }
    }    
else if(type == __INT64){
        if(!regex_match(str, regex("^-?\\d+"))){
            cout << "<E> PARAMETER '" << str << "' MISMATCH TYPE 'INT64'." << endl;
            return false;
        }
    }    
else if(type == __REAL){
        if(!regex_match(str, regex("^-?\\d+\\.\\d+")) && !regex_match(str, regex("^-?\\d+"))){
            cout << "<E> PARAMETER '" << str << "' MISMATCH TYPE 'REAL'." << endl;
            return false;
        }
    }
    else if(type == __TEXT){
        if(str.length() > TEXT_LENGTH - 1){
            cout << "<E> OUT OF RANGE 'TEXT'" << endl;
            return false;
        }
    }
    else{
        if(str.length() > LONGTEXT_LENGTH - 1){
            cout << "<E> OUT OF RANGE 'LONGTEXT'" << endl;
            return false;
        }
    }
    return true;
}


