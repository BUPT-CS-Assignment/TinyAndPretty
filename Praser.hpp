#ifndef __PRASER_HPP__
#define __PRASER_HPP__
#include<iostream>
using namespace std;

/*
    解析器-头文件
    类名称：InputStream
*/

//元命令枚举类型
enum __COMMAND__{__UNK,__EXIT,__HELP,__OPERATE};
//操作命令枚举类型
enum __OPERATE__{__UNKNOWN,__SAVE,__TABLE_CREATE,__INSERT,__DELETE,__REPLACE,__PRINT};   

class InputStream{
private:
    __OPERATE__  __Operate;   //操作
    string __Statement;   //内容
    string __Input;   //所有输入
    /*
        元命令/操作命令解析
    */
    __COMMAND__  Parser(string input){
        if(input[0] != '.') return __OPERATE;
        if(input.compare(".exit") == 0) return __EXIT;
        if(input.compare(".help") == 0) return __HELP;
        return __UNK;
    }

    __OPERATE__  Instruct(string input){
        string command = input.substr(0,input.find(" ")+1);
        if(command.compare("INSERT ") == 0) return __INSERT;
        if(command.compare("SAVE ") == 0) return __SAVE;
        if(command.compare("CREATE ") == 0) return __TABLE_CREATE;
        if(command.compare("DELETE ") == 0) return __DELETE;
        if(command.compare("PRINT ") == 0) return __PRINT;
        return __UNKNOWN;

    }

public:
    //初始化
    InputStream(){
        __Operate = __UNKNOWN;
        __Statement = "";
        __Input = "";
    }
    //读入函数
    __COMMAND__ Read(){
        getline(cin,__Input);
        __COMMAND__ command = Parser(__Input);   //获取元命令类型
        if(command == __OPERATE){
            __Operate = Instruct(__Input);   //获取操作命令类型 
            if(__Operate != __UNKNOWN){
                int index = __Input.find(" ");
                __Input = __Input.substr(index+1,__Input.length()-index-1);
                /*
                    多行读取
                    分号结束
                */
                while(true){
                    index = __Input.find(';');
                    if(index != -1){
                        __Statement = __Statement +__Input.substr(0,index);
                        fflush(stdin);
                        break;
                    }else{
                        __Statement = __Statement + __Input;
                        printf("      -> ");
                        getline(cin,__Input);
                    }
                }
            }else{
                printf("Unknown Operation '%s'\n",__Input.c_str());
            }
        }
        return command;
    }
    string getInput(){
        return __Input;
    }

    string getStatement(){
        return __Statement;
    }

    void clearStatement(){
        __Statement = "";
    }

    __OPERATE__ getOperation(){
        return __Operate;
    }

};

#endif