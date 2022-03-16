#ifndef __IMPLEMENT_H__
#define __IMPLEMENT_H__
#include<iostream>
#include<string.h>
#include<Basic.hpp>
using namespace std;
/*

    Split(string str,char c,int& length)  @  字符串拆分函数
    str : 需拆分字符串
    c : 拆分字符索引
    length : 拆分后字符串数组长度

*/ 
string* Split(string str,char c,int &length);
bool to_Int(string str,int& res);
string Trim(string str);
bool OverflowCheck(string str,__DataType__ type);

#endif