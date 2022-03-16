#include<Implement.h>
#include<regex>

string* Split(string str,char c,int &length){
    int count = 1;   // 拆分部分数量
    int index = 0;   // 字符索引位置
    int l = str.length();
    index = str.find(c,index);
	while (index < l && index >=0){
		count++;
		index++;
        index = str.find(c,index);
	}
    string* words = new string[count];   //拆分部分数组
    int start = 0, end = str.find(c, start);
    for(int i = 0;i<count;i++){
        words[i] = Trim(str.substr(start,end-start));
        start = end + 1;
        end = str.find(c,start);
        if(end == -1) end = str.length();
    }
    length = count;
    return words;
}

string Trim(string str){
    string temp = str;
    temp.erase(0, temp.find_first_not_of(" "));
    temp.erase(temp.find_last_not_of(" ")+1);
    return temp;
}

bool to_Int(string str,int& res){
    str = Trim(str);
    int positive = 1;
    if(str[0] == '-'){
        positive = -1;
        str = str.substr(1,str.length()-1);
    }
    if(str.length() == 0){
        cout<<"Parameter Empty."<<endl;
        return false;
    }
    if(regex_search(str,regex("\\D+")) > 0){
        cout<<"Parameter '"<<str<<"' Mismatch Type : 'int'."<<endl;
        return false;
    }
    res = positive * stoi(str.c_str());
    return true;
}

bool OverflowCheck(string str,__DataType__ type){
    if(type == __Short){
        if(str.compare("32767") > 0 ||
          (str[0] == '-' && str.compare("-32768") > 0)){
            cout<<"Overflow! <Type : short>"<<endl; 
            return false;
        }
    }else if(type == __Int){
        if(str.compare("2147483647") > 0 ||
          (str[0] == '-' && str.compare("-2147483648") > 0)){
            cout<<"Overflow! <Type : int>"<<endl; 
            return false;
        }
    }else if(type == __UInt){
        if(str.compare("4294967295") > 0 || str[0] == '-' ){
            cout<<"Overflow! <Type : uint>"<<endl; 
            return false;
        }
    }else if(type == __Char){
        if(str.length() != 1){
            cout<<"Overflow! <Type : char>"<<endl; 
            return false;
        }
    }else if(type == __VarChar11){
        if(str.length() >= 11){
            cout<<"Overflow! <Type : varchar11>"<<endl; 
            return false;
        }
    }else if(type == __VarChar32){
        if(str.length() > 32){
            cout<<"Overflow! <Type : varchar32>"<<endl; 
            return false;
        }
    }else if(type == __VarChar255){
        if(str.length() > 255){
            cout<<"Overflow! <Type : varchar255>"<<endl; 
            return false;
        }
    }
    return true;

            
}