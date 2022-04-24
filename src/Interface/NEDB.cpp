#include<Interface/NEDB.h>
#include<Basic/process.h>
#include<Utils/implement.h>
#include<Basic/data.h>
using namespace std;

int MAX_TABLES = 64;          /* Max Table Numbers */
int DEFAULT_PAGE_SIZE = 400;  /* Default Page Size */
int SIG_WAIT_MSECS = 100;     /* Time(ms) Wait For Unlock */
int SIG_CHECK_TIMES = 5;      /* Check Times While Waiting */
int SIG_DEBUG = 0;

void NEDB_SETTING(int max_tables, int default_page_size, int sig_wait_msecs, int sig_check_times){
    MAX_TABLES = max_tables;                /* Max Table Numbers */
    DEFAULT_PAGE_SIZE = default_page_size;  /* Default Page Size */
    SIG_WAIT_MSECS = sig_wait_msecs;        /* Time(ms) Wait For Unlock */
    SIG_CHECK_TIMES = sig_check_times;      /* Check Times While Waiting */
}

void NEDB_DEBUG(int debug){
    SIG_DEBUG = debug;
}

NEDB::NEDB(const char* dir){
    nedb = new DataBase(dir);
}

const string NEDB::GetDir(){
    if(nedb == NULL) return "";
    const string str = nedb->getDir();
    return str;
}

int NEDB::DefaultPageSize(){
    if(nedb == NULL) return -1;
    return nedb->getDefaultPageSize();
}

int NEDB::SetDefaultPageSize(int size){
    if(nedb == NULL) return -1;
    return nedb->setDefaultPageSize(size);
}

int NEDB::SetDir(const char* dir){
    if(nedb == NULL) return -1;
    return nedb->setDir(dir);
}

int NEDB::ErrCode(){
    if(nedb == NULL) return -1;
    return nedb->getErrCode();
}

const string NEDB::ReturnVal(){
    if(nedb == NULL) return "";
    const string str = nedb->getReturnVal();
    return str;
}

int NEDB::Count(){
    if(nedb == NULL) return -1;
    return nedb->getCount();
}

int NEDB::DirInit(){
    if(nedb == NULL) return -1;
    return nedb->dirInit();
}

int NEDB::Openall(){
    if(nedb == NULL) return -1;
    return nedb->openall();
}

int NEDB::Open(const char* fileName){
    if(nedb == NULL) return -1;
    return nedb->open(fileName);
}

int NEDB::Exec(const char* sql){
    if(nedb == NULL) return -1;
    return nedb->exec(sql);
}

int NEDB::Close(){
    if(nedb == NULL) return -1;
    int res = nedb->close();
    delete nedb;
    return res;
}