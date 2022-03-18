#include<Implement.h>


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
    }else{
        int length[10];
        length[__Char] = 1; 
        length[__VarChar11] = 11;
        length[__VarChar32] = 32;
        length[__VarChar255] = 255;
        if(str.length() > length[type]){
            cout<<"Overflow! <Type : "<<__Type__[type]<<">"<<endl; 
            return false;
        }
    }
    return true;        
}

template<class T>
void sort(T* array,int l, int r){
    if(l>=r) return;
    int blank = l +rand()%(r-l+1);
    int i = l,j = r;
    T key = array[blank];
    while(i<j){
        while(j>blank){
            if(array[j]>key){
                array[blank]=array[j];
                blank = j;
            }else j--;
        }
        while(i<blank){
            if(array[i]<key){
                array[blank]=array[i];
                blank = i;
            }else i++;
        }
    }
    array[blank]=key;
    qsort(N,l,blank-1);
    qsort(N,blank+1,r);
}

