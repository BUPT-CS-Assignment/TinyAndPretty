#ifndef __IMPLEMENT_H__
#define __IMPLEMENT_H__
#include<iostream>
#include<basic.h>
using namespace std;
/*

    Split(string str,char c,int& length)  @  字符串拆分函数
    str : 需拆分字符串
    c : 拆分字符索引
    length : 拆分后字符串数组长度

*/ 
string* Split(string str,char c,int &length);
void parm_check(string str,DATA_TYPE type);
string Trim(string str);

template<class T>
void q_sort(T** array,int l, int r){
    if(l>=r) return;
    int blank = l +rand()%(r-l+1);
    int i = l,j = r;
    T* key = array[blank];
    while(i<j){
        while(j>blank){
            if(*(array[j])<*key){
                array[blank]=array[j];
                blank = j;
            }else j--;
        }
        while(i<blank){
            if(*(array[i])>*key){
                array[blank]=array[i];
                blank = i;
            }else i++;
        }
    }
    array[blank]=key;
    q_sort(array,l,blank-1);
    q_sort(array,blank+1,r);
}

#endif