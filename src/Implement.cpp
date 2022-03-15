#include<Implement.h>
#include <regex>

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
        words[i] = str.substr(start,end-start);
        start = end + 1;
        end = str.find(c,start);
        if(end == -1) end = str.length();
    }
    length = count;
    return words;
}

bool to_Int(string str,int& res){
    if(str.length() == 0){
        cout<<"Parameter Empty."<<endl;
        return false;
    }
    if(regex_search(str,regex("\\D+")) > 0){
        cout<<"Parameter '"<<str<<"' Mismatch Type : 'int'."<<endl;
        return false;
    }
    res = stoi(str.c_str());
    return true;
}
