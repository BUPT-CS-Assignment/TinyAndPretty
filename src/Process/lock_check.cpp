#include<Basic/process.h>
#include<main.h>
using namespace std;


int StatusCheck(int& state,int level,int times){
    if(state == SIG_BLOCK) return 0;
    if(state >= level) return 1;
    for(int i = 0; i <times; i++){
        ConsoleLog(0,DEBUG_SIMPLE,"(NEDB)Waiting...\n");
        usleep(SIG_WAIT_MSECS * 1000);
        if(state >= level){
            return 1;
        }
    }
    return ConsoleLog(0,DEBUG_SIMPLE,"(NEDB)Task Cancelled\n");
}