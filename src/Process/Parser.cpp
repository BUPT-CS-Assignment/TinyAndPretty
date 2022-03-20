#include<Process.h>
#include<Implement.h>

Parser::Parser(){
    __Statement = "";
    __Operate = __UNKNOWN;
    __Command = __UNK;
}

void Parser::analyse(string input){
    if(input[0] != '.'){
        __Command = __OPERATE;
        __Operate = operate_type(input); 
        int index = input.find(" ");
        if(index <= 0){
            __Statement = input;
        }else{
           if(__Operate == __UNKNOWN){
               __Statement = input.substr(0,index);
           }else{
               __Statement = input.substr(index+1,input.length()-index-1);
           } 
        }
    }else if(input.compare(".exit") == 0){
        __Command = __EXIT;
    }else if(input.compare(".help") == 0){
        __Command = __HELP;
    }else if(input.compare(".save") == 0){
        __Command = __SAVE;
    }else{
        __Command = __UNK;
        __Statement = input;
    }
    
}


__OPERATE__  Parser::operate_type(string input){
    if(input.compare("SHOW TABLES") == 0) return __SHOW; 
    string command = input.substr(0,input.find(" "));
    //
    /******************/
    if(command.compare("CREATE") == 0) return __TABLE_CREATE;
    if(command.compare("REMOVE") == 0) return __TABLE_REMOVE; 
    if(command.compare("INSERT") == 0) return __INSERT;
    if(command.compare("DELETE") == 0) return __DELETE;
    if(command.compare("PRINT") == 0) return __PRINT;
    return __UNKNOWN;
}

string Parser::getStatment(){
    return __Statement;
}

__COMMAND__ Parser::getCommand(){
    return __Command;
}

__OPERATE__ Parser::getOperate(){
    return __Operate;
}

