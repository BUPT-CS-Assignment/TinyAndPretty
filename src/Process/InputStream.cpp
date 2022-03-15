#include <Process.h>
#include <DataBase.h>

/*
    解析器-头文件
    类名称：InputStream
*/

__COMMAND__  InputStream::Parser(string input){
    if(input[0] != '.') return __OPERATE;
    if(input.compare(".exit") == 0) return __EXIT;
    if(input.compare(".help") == 0) return __HELP;
    if(input.compare(".save") == 0) return __SAVE;
    return __UNK;
}

__OPERATE__  InputStream::Instruct(string input){
    if(input.compare("SHOW TABLES;") == 0) return __SHOW; 
    string command = input.substr(0,input.find(" ")+1);
    if(command.compare("CREATE ") == 0) return __TABLE_CREATE;
    if(command.compare("REMOVE ") == 0) return __TABLE_REMOVE; 
    if(command.compare("INSERT ") == 0) return __INSERT;
    if(command.compare("DELETE ") == 0) return __DELETE;
    if(command.compare("PRINT ") == 0) return __PRINT;
    return __UNKNOWN;
}

//初始化
InputStream::InputStream(){
    __Operate = __UNKNOWN;
    __Statement = "";
    __Input = "";
}
//读入函数
__COMMAND__ InputStream::Read(){
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

string InputStream::getInput(){
    return __Input;
}

string InputStream::getStatement(){
    return __Statement;
}

void InputStream::ClearStatement(){
    __Statement = "";
}

__OPERATE__ InputStream::getOperation(){
        return __Operate;
    }





