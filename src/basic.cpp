#include<main.h>
#include<Basic/process.h>
#include<Utils/implement.h>
#include<Basic/data.h>
using namespace std;

DataBase::DataBase(string dir){
    if(dir.length() == 0 || dir[dir.length() - 1] != '/'){
        dir = dir + "/";
    }
    __SrcDir__ = dir;
    __Tables__ = new Table * [MAX_TABLES];
    __PageSize__ = DEFAULT_PAGE_SIZE;
    __Cursor__ = 0;
    __ErrCode__ = NO_ERROR;
    __ReturnVal__ = "";
    __OperateCount__ = 0;
    __DeleteLock__ = SIG_UNLOCK;
    __ActionLock__ = SIG_UNLOCK;
}

int DataBase::setDir(string dir){
    if(dir.length() == 0){
        __ErrCode__ = DIR_ERROR;
        return DIR_ERROR;
    }
    if(dir[dir.length() - 1] != '/'){
        dir = dir + "/";
    }
    __SrcDir__ = dir;
    __ErrCode__ = NO_ERROR;
    return NO_ERROR;
}

string DataBase::getDir(){
    return __SrcDir__;
}

string DataBase::getReturnVal(){
    return __ReturnVal__;
}

int DataBase::getErrCode(){
    return __ErrCode__;
}

int DataBase::getDefaultPageSize(){
    return __PageSize__;
}

void DataBase::setErrCode(int code){
    __ErrCode__ = code;
}
void DataBase::setCount(int count){
    __OperateCount__ = count;
}

int DataBase::getCount(){
    return __OperateCount__;
}

void DataBase::AddCount(){
    __OperateCount__ ++;
}

void DataBase::SubCount(){
    __OperateCount__ --;
}

Table* DataBase::getTable(int i){
    if(i >= __Cursor__) return NULL;
    return __Tables__[i];
}

int DataBase::getCursor(){
    return __Cursor__;
}

void DataBase::setReturnVal(string val){
    __ReturnVal__ = val;
}



int DataBase::setDefaultPageSize(int size){
    if(size < 100 || size > 4000){
        __ErrCode__ = SIZE_NOT_ALLOWED;
        return SIZE_NOT_ALLOWED;
    }
    __PageSize__ = size;
    __ErrCode__ = NO_ERROR;
    return NO_ERROR;
}

void DataBase::addTable(Table* table){
    __Tables__[__Cursor__] = table;
    __Cursor__ ++;
}

void DataBase::dropTable(string name){
    try{
        __DeleteLock__ = SIG_LOCK;
        if(__LockCheck__(__ActionLock__, SIG_CHECK_TIMES * 2) != SIG_UNLOCK){
            __DeleteLock__ = SIG_UNLOCK;
            throw ACTION_BUSY;
        }
        for(int i = 0; i < __Cursor__; i++){
            if(__Tables__[i] == NULL){
                __DeleteLock__ = SIG_UNLOCK;
                throw SYSTEM_ERROR;
            }
            string temp = __Tables__[i]->getName();
            if(temp == name){
                __Tables__[i]->Erase();
                Memorizer RAM(__Tables__[i]);
                RAM.TableDrop();
                delete[] __Tables__[i];
                for(int j = i; j < __Cursor__ - 1; j++){
                    __Tables__[j] = __Tables__[j + 1];
                }
                __Tables__[__Cursor__ - 1] = NULL;
                -- __Cursor__;
                __DeleteLock__ = SIG_UNLOCK;
                return;
            }
        }
        __DeleteLock__ = SIG_UNLOCK;
        throw TABLE_NOT_FOUND;
    }
    catch(NEexception& e){
        __DeleteLock__ = SIG_UNLOCK;
        throw e;
    }
}

int DataBase::dirInit(){
    try{
        int dir_num = 0;
        string* dirs = Split(__SrcDir__, '/', dir_num);
        string dir = "";
        for(int i = 1; i < dir_num - 1; i++){
            dir = dir + "/" + dirs[i];
            if(NULL == opendir(dir.c_str())){
                if(mkdir(dir.c_str(), S_IRWXU) == -1){
                    throw DIR_ERROR;
                }
            }
        }
        __ErrCode__ = NO_ERROR;
        return __ErrCode__;
    }
    catch(NEexception& e){
        __ErrCode__ = e;
        return __ErrCode__;
    }
}

int DataBase::openall(){
    try{
        DIR* dp = opendir(__SrcDir__.c_str());
        __OperateCount__ = 0;
        if(dp == NULL){
            throw DIR_ERROR;
        }
        regex layout(".+\\.nef");
        struct dirent* dirfiles;
        while((dirfiles = readdir(dp)) != NULL){
            if(__Cursor__ >= MAX_TABLES){
                throw TABLE_NUM_REACH_LIMIT;
            }
            string file_name = dirfiles->d_name;
            if(regex_match(file_name, layout)){
                file_name = file_name.substr(0, file_name.find("."));
                if(open(file_name) != NO_ERROR){
                    __ErrCode__ = FILE_DAMAGED;
                    return __ErrCode__;
                }
                __OperateCount__ ++;
            }
        }
        closedir(dp);
        __ErrCode__ = NO_ERROR;
        __ReturnVal__ = "";
        return __ErrCode__;
    }
    catch(NEexception& e){
        __ErrCode__ = e;
        __ReturnVal__ = "";
        return __ErrCode__;
    }
}

int DataBase::open(string name){
    try{
        if(SIG_DEBUG == 1){
            cout << "(NEDB)Loading '" << name << "'" << endl;
        }
        Memorizer RAM(NULL);
        Table* table = NULL;
        if(__Cursor__ >= MAX_TABLES){
            throw TABLE_NUM_REACH_LIMIT;
        }
        table = RAM.TableLoad(this, name);
        if(getTable(name) == NULL){
            __Tables__[__Cursor__] = table;
            ++__Cursor__;
        }
        __ErrCode__ = NO_ERROR;
        return __ErrCode__;
    }
    catch(NEexception& e){
        __ErrCode__ = e;
        return __ErrCode__;
    }
}

int DataBase::exec(string sql){
    __ReturnVal__ = "";
    __ErrCode__ = NO_ERROR;
    __OperateCount__ = 0;
    if(sql.length() == 0 || sql[sql.length() - 1] != ';'){
        __ErrCode__ = SQL_FORM_ERROR;
        return 0;
    }
    /////
    Parser p;
    Executor e(&p, this);
    int sql_num;
    setReturnVal("");
    string res = "";
    string* sqls = Split(sql, ';', sql_num);
    int t = 0;
    try{
        for(int i = 0; i < sql_num - 1; i++){
            p.i_analyse(sqls[i]);
            e.execute_operation();
            string temp = getReturnVal();
            if(temp.length() != 0){
                if(t == 0){
                    res = temp;
                    t ++;
                }
                else res = res + "\n" + temp;
            }
            setReturnVal("");
            p.flush();
        }
        __ReturnVal__ = res;
        __ErrCode__ = NO_ERROR;
        return __ErrCode__;
    }
    catch(NEexception& E){
        __ReturnVal__ = res;
        __ErrCode__ = E;
        return __ErrCode__;
    }
}

int DataBase::close(){
    try{
        if(SIG_DEBUG == 1){
            cout << "(NEDB)Closing..." << endl;
        }
        __DeleteLock__ = SIG_LOCK;
        __ActionLock__ = SIG_LOCK;
        for(int i = 0; i < __Cursor__; i++){
            __Tables__[i]->Erase();
        }
        delete[] __Tables__;
        __Cursor__ = -1;
        __Tables__ = NULL;
        __ErrCode__ = NO_ERROR;
        __DeleteLock__ = SIG_UNLOCK;
        __ActionLock__ = SIG_UNLOCK;
        return __ErrCode__;
    }
    catch(exception& e){
        __DeleteLock__ = SIG_UNLOCK;
        __ActionLock__ = SIG_UNLOCK;
        __ErrCode__ = SYSTEM_ERROR;
        return __ErrCode__;
    }
}

Table* DataBase::getTable(string name){
    try{
        for(int i = 0; i < __Cursor__; i++){
            if(__Tables__[i] == NULL){
                throw SYSTEM_ERROR;
            }
            string temp = __Tables__[i]->getName();
            if(temp == name) return __Tables__[i];
        }
        return NULL;
    }
    catch(NEexception& e){
        throw e;
    }
}
