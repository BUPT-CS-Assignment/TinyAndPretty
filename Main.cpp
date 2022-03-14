#include<iostream>
using namespace std;
enum __COMMAND__{__UNK,__EXIT,__HELP,__OPERATE};
enum __OPERATE__{__UNKNOWN,__SAVE,__TABLE_CREATE,__INSERT,__DELETE,__REPLACE};
/*
    InputStream 类：
        输入内容
        命令格式
*/

class InputStream{
private:
    __OPERATE__  __Operate;
    string __Statement;
    string __Input;

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
        return __UNKNOWN;

    }

public:

    InputStream(){
        __Operate = __UNKNOWN;
        __Statement = "";
        __Input = "";
    }

    __COMMAND__ Read(){
        //scanf("%[^\n]%*c",__Input);
        getline(cin,__Input);
        __COMMAND__ command = Parser(__Input);
        if(command == __OPERATE){
            __Operate = Instruct(__Input);
            if(__Operate != __UNKNOWN){
                int index = __Input.find(" ");
                __Input = __Input.substr(index+1,__Input.length()-index-1);
                while(true){
                    if(__Input[__Input.length()-1] == ';'){
                        __Statement = __Statement +__Input.substr(0,__Input.length()-2);
                        break;
                    }else{
                        __Statement = __Statement + __Input;
                        printf("      -> ");
                        getline(cin,__Input);
                        //scanf("%[^\n]%*c",__Input);
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

};

int main(/*int argc, char* argv[]*/){
    InputStream inputStr;
    while(true){
        printf("LiteDB > ");
        switch (inputStr.Read()){
        case __EXIT : 
            printf("~\n");
            return 0;
        case __UNK :
            printf("Unknown Command '%s'\n",inputStr.getInput().c_str());
            break;
        case __HELP :
            printf("This is a help prompt.\n");
            break;
        default :
            break;
        }
    }
}