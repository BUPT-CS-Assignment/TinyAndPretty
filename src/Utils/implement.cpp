#include<main.h>
using namespace std;
using namespace NEDBnamespace;

int NEDBnamespace::StatusCheck(int& state,int level,int times){
    if(state == SIG_BLOCK) return 0;
    if(state >= level) return 1;
    bool flag = SIG_DEBUG>=DEBUG_SIMPLE;
    for(int i = 0; i <times; i++){
        CONSOLE_LOG(0,TIME_FLAG,flag,"(NEDB)Waiting...\n");
        usleep(SIG_WAIT_MSECS * 1000);
        if(state >= level){
            return 1;
        }
    }
    return CONSOLE_LOG(0,TIME_FLAG,flag,"(NEDB)Task Cancelled\n");
}

void NEDBnamespace::parm_check(string str, DATA_TYPE type){
    if(str.length() == 0){
        throw PARAM_EMPTY;
    }
    if(type == __INT){
        if(!regex_match(str, regex("^-?\\d+"))){
            throw TYPE_INT_MISMATCH;
        }
    }
    else if(type == __INT64){
        if(!regex_match(str, regex("^-?\\d+"))){
            throw TYPE_INT64_MISMATCH;
        }
    }
    else if(type == __REAL){
        if(!regex_match(str, regex("^-?\\d+\\.\\d+")) && !regex_match(str, regex("^-?\\d+"))){
            throw TYPE_REAL_MISMATCH;
        }
    }
    else if(type == __TEXT){
        if(str.length() > TEXT_LENGTH - 1){
            throw TYPE_TEXT_OVERFLOW;
        }
    }
    else{
        if(str.length() > LONGTEXT_LENGTH - 1){
            throw TYPE_LONGTEXT_OVERFLOW;
        }
    }
}


