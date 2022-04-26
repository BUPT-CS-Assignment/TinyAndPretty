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
    __ProcStatus__ = SIG_FREE;
}

int DataBase::setDir(string dir){
    if(dir.length() == 0){
        __ErrCode__ = DIR_ERROR;
        return DIR_ERROR;
    }
    if(dir[dir.length() - 1] != '/'){
        dir = dir + "/";
    }
    ConsoleLog(0, DEBUG_DETAIL, "(NEDB)Dir Set to '%s'\n", dir.c_str());
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
    ConsoleLog(0, DEBUG_DETAIL, "(NEDB)Set Page Size to %d\n", size);
    __PageSize__ = size;
    __ErrCode__ = NO_ERROR;
    return NO_ERROR;
}

int& DataBase::Status(){
    return __ProcStatus__;
}

void DataBase::SetStatus(int state){
    __ProcStatus__ = state;
}

void DataBase::addTable(Table* table){
    __Tables__[__Cursor__] = table;
    __Cursor__ ++;
}

void DataBase::dropTable(string name){
    try{
        /* DataBase Lock Check */
        if(!StatusCheck(Status(), SIG_FREE, SIG_CHECK_TIMES)){
            throw ACTION_BUSY;
        }
        SetStatus(SIG_BLOCK);
        for(int i = 0; i < __Cursor__; i++){
            if(__Tables__[i] == NULL){
                SetStatus(SIG_FREE);
                throw SYSTEM_ERROR;
            }
            string temp = __Tables__[i]->getName();
            if(temp == name){
                Table* temp = __Tables__[i];
                while(1){
                    if(StatusCheck(temp->table_status_,SIG_FREE,1)){
                        temp->table_status_ = SIG_BLOCK;
                        break;
                    }
                }
                __Tables__[i] = NULL;
                for(int j = i; j < __Cursor__ - 1; j++){
                    __Tables__[j] = __Tables__[j + 1];
                }
                __Tables__[__Cursor__ - 1] = NULL;
                -- __Cursor__;
                /* Memory Free */
                temp->Erase();
                Memorizer RAM(temp);
                RAM.TableDrop();
                delete[] temp;
                SetStatus(SIG_FREE);
                return;
            }
        }
        SetStatus(SIG_FREE);
        throw TABLE_NOT_FOUND;
    }
    catch(NEexception& e){
        SetStatus(SIG_FREE);
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
                    ConsoleLog(0, DEBUG_DETAIL, "(NEDB)Dir '%s' Error\n", dir.c_str());
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
                    ConsoleLog(0, DEBUG_DETAIL, "(NEDB)File Damaged : '%s'\n", file_name.c_str());
                    __ErrCode__ = FILE_DAMAGED;
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
        /* Thread Lock Check */
        if(!StatusCheck(Status(), SIG_FREE, SIG_CHECK_TIMES)){
            throw ACTION_BUSY;
        }
        ConsoleLog(0, DEBUG_SIMPLE, "(NEDB)Loading '%s'\n", name.c_str());
        /* Read File */
        Memorizer RAM(NULL);
        Table* table = NULL;
        if(__Cursor__ >= MAX_TABLES){
            throw TABLE_NUM_REACH_LIMIT;
        }
        SetStatus(SIG_BLOCK);
        table = RAM.TableLoad(this, name);
        if(getTable(name) == NULL){
            __Tables__[__Cursor__] = table;
            ++__Cursor__;
            SetStatus(SIG_FREE);
        }
        SetStatus(SIG_FREE);
        __ErrCode__ = NO_ERROR;
        return NO_ERROR;
    }
    catch(NEexception& e){
        SetStatus(SIG_FREE);
        __ErrCode__ = e;
        return e;
    }
}

int DataBase::exec(string sql){
    __ReturnVal__ = "";
    __ErrCode__ = NO_ERROR;
    __OperateCount__ = 0;
    if(sql.length() == 0 || sql[sql.length() - 1] != ';'){
        __ErrCode__ = SQL_FORM_ERROR;
        return __ErrCode__;
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
        ConsoleLog(0, DEBUG_NONE, "(NEDB)Closing...\n");
        if(!StatusCheck(Status(), SIG_FREE, SIG_CHECK_TIMES)){
            throw ACTION_BUSY;
        }
        SetStatus(SIG_BLOCK);
        usleep(SIG_WAIT_MSECS * SIG_CHECK_TIMES * 1000);
        /////
        for(int i = 0; i < __Cursor__; i++){
            if(__Tables__[i] != NULL){
                __Tables__[i]->Erase();
            }
        }
        delete[] __Tables__;
        __Cursor__ = -1;
        __Tables__ = NULL;
        __ErrCode__ = NO_ERROR;
        return __ErrCode__;
    }
    catch(exception& e){
        SetStatus(SIG_FREE);
        __ErrCode__ = SYSTEM_ERROR;
        return __ErrCode__;
    }
}

Table* DataBase::getTable(string name){
    try{
        //SetStatus(SIG_RUN);
        for(int i = 0; i < __Cursor__; i++){
            if(__Tables__[i] == NULL){
                //SetStatus(SIG_FREE);
                throw SYSTEM_ERROR;
            }
            string temp = __Tables__[i]->getName();
            if(temp == name){
                //SetStatus(SIG_FREE);
                return __Tables__[i];
            }
        }
        //SetStatus(SIG_FREE);
        return NULL;
    }
    catch(NEexception& e){
        //SetStatus(SIG_FREE);
        throw e;
    }
}
