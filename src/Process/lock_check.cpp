#include<Basic/process.h>
#include<main.h>
using namespace std;

int __LockCheck__(int& lock, int times){
    if(lock == SIG_UNLOCK)  return SIG_UNLOCK;
    for(int i = 0; i < times; i++){
        if(SIG_DEBUG == 1){
            cout << "(NEDB)Waiting..." << endl;
        }
        usleep(SIG_WAIT_MSECS * 1000);
        if(lock == SIG_UNLOCK){
            return SIG_UNLOCK;
        }
    }
    if(SIG_DEBUG == 1){
        cout << "(NEDB)Task Cancelled" << endl;
    }
    return SIG_LOCK;
}