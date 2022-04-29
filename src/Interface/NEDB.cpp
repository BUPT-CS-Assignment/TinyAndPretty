#include<Interface/NEDB.h>
#include<Basic/process.h>
#include<Basic/data.h>
using namespace std;
using namespace NEDBnamespace;

int NEDBnamespace::MAX_TABLES = 64;          /* Max Table Numbers */
int NEDBnamespace::DEFAULT_PAGE_SIZE = 400;  /* Default Page Size */
int NEDBnamespace::SIG_WAIT_MSECS = 200;     /* Time(ms) Wait For Unlock */
int NEDBnamespace::SIG_CHECK_TIMES = 5;      /* Check Times While Waiting */
int NEDBnamespace::SIG_DEBUG = 0;            /* Set Debug Flag */
int NEDBnamespace::TIME_FLAG = 1;            /* Set Time-Show Flag */
string NEDBnamespace::DEFAULT_DIR = "/";

void NEDBSTD::NEDB_SETTING(int max_tables, int default_page_size, int sig_wait_msecs, int sig_check_times){
    MAX_TABLES = max_tables;                /* Max Table Numbers */
    DEFAULT_PAGE_SIZE = default_page_size;  /* Default Page Size */
    SIG_WAIT_MSECS = sig_wait_msecs;        /* Time(ms) Wait For Unlock */
    SIG_CHECK_TIMES = sig_check_times;      /* Check Times While Waiting */
}

void NEDBSTD::NEDB_DEBUG(int debug){
    SIG_DEBUG = debug;
}

void NEDBSTD::NEDB_TIME_FLAG(bool flag){
    TIME_FLAG = flag;
}

NEDB::NEDB(const string dir){
    nedb = new DataBase(dir);
}

int NEDB::Close(){
    if(nedb == NULL) return 1;
    int res = nedb->close();
    delete nedb;
    return res;
}

int NEDB::MountAll(int& count){
    if(nedb == NULL) return 1;
    return nedb->openall(count);
}

int NEDB::Open(const string filePath){
    if(nedb == NULL) return 1;
    return nedb->open(filePath,FULL_PATH);
}

int NEDB::Mount(const string fileName){
    if(nedb == NULL) return 1;
    return nedb->open(fileName,RELATIVE_PATH);
}

int NEDB::Unmount(const string tableName){
    if(nedb == NULL) return 1;
    try{
        nedb->dropTable(tableName,FILE_UNMOUNT);
        return NO_ERROR;
    }catch(NEexception &e){
        return e;
    }
}

int NEDB::Drop(const string tableName){
    if(nedb == NULL) return 1;
    try{
        nedb->dropTable(tableName,FILE_DROP);
        return NO_ERROR;
    }catch(NEexception &e){
        return e;
    }
}

int NEDB::Query(const string sql,int& count,string& retVal ){
    if(nedb == NULL) return 1;
    return nedb->exec(sql,count,retVal);
}

int NEDB::Create(const string name,const string setField){
    if(nedb == NULL) return 1;
    return nedb->create_table(name,setField);
}

int NEDB::Describe(const string name,string& res){
    if(nedb == NULL) return 1;
    return nedb->describe_table(name,res);

}

int NEDB::Insert(const string table, const string fields, const string values){
    if(nedb == NULL) return 1;
    return nedb->insert_into(table,fields,values);
}

int NEDB::Delete(const std::string table, const std::string conditions, int& count){
    if(nedb == NULL) return 1;
    return nedb->delete_from(table,conditions,count);
}

int NEDB::Select(const string table, const string fields, const string conditions, int& count, string& retVal){
    if(nedb == NULL) return 1;
    return nedb->select_from(table,fields,conditions,count,retVal);
}

int NEDB::Update(const string table, const string setVal, const string conditions, int& count){
    if(nedb == NULL) return 1;
    return nedb->update_set(table,setVal,conditions,count);
}

int NEDB::DirInit(){
    if(nedb == NULL) return 1;
    return nedb->dirInit();
}

int NEDB::SetDir(const string dir){
    if(nedb == NULL) return 1;
    return nedb->setDir(dir);
}

const string NEDB::GetDir(){
    if(nedb == NULL) return "";
    return nedb->getDir();
}

int NEDB::SetDefaultPageSize(int size){
    if(nedb == NULL) return 1;
    return nedb->setDefaultPageSize(size);
}

int NEDB::DefaultPageSize(){
    if(nedb == NULL) return 1;
    return nedb->getDefaultPageSize();
}












