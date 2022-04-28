#include<main.h>
#include<Basic/process.h>
#include<Basic/data.h>
using namespace std;
using namespace NEDBnamespace;


DataBase::DataBase(string dir){
    if(dir.length() == 0 || dir[dir.length() - 1] != '/'){
        dir = dir + "/";
    }
    __SrcDir__ = dir;
    __Tables__ = new Table * [MAX_TABLES];
    __PageSize__ = DEFAULT_PAGE_SIZE;
    __Cursor__ = 0;
    __ProcStatus__ = SIG_FREE;
}

int DataBase::setDir(string dir){
    if(dir.length() == 0){
        return DIR_ERROR;
    }
    if(dir[dir.length() - 1] != '/'){
        dir = dir + "/";
    }
    int flag = SIG_DEBUG >= DEBUG_DETAIL;
    CONSOLE_LOG(0,TIME_FLAG,flag, "(NEDB)Dir Set '%s'\n", dir.c_str());
    __SrcDir__ = dir;
    return NO_ERROR;
}

string DataBase::getDir(){
    return __SrcDir__;
}

int DataBase::getDefaultPageSize(){
    return __PageSize__;
}


Table* DataBase::getTable(int i){
    if(i >= __Cursor__) return NULL;
    return __Tables__[i];
}

int DataBase::getCursor(){
    return __Cursor__;
}



int DataBase::setDefaultPageSize(int size){
    if(size < 100 || size > 4000){
        return SIZE_NOT_ALLOWED;
    }
    bool flag = SIG_DEBUG >=DEBUG_DETAIL;
    CONSOLE_LOG(0,TIME_FLAG,flag,"(NEDB)PageSize Set '%d' Bytes\n", size);
    __PageSize__ = size;
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

void DataBase::dropTable(string name,int mode){
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
                    if(StatusCheck(temp->table_status_, SIG_FREE, 1)){
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
                SetStatus(SIG_FREE);
                /* Memory Free */
                temp->Erase();
                if(mode == FILE_DROP){
                    Memorizer RAM(temp);
                    RAM.TableDrop();
                }
                delete temp;
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
                    int flag = SIG_DEBUG >= DEBUG_DETAIL;
                    CONSOLE_LOG(0,TIME_FLAG,flag, "(NEDB)Dir '%s' Error\n", dir.c_str());
                    throw DIR_ERROR;
                }
            }
        }
        return NO_ERROR;
    }
    catch(NEexception& e){
        return e;
    }
}

int DataBase::openall(int& num){
    try{
        num = 0;
        DIR* dp = opendir(__SrcDir__.c_str());
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
                if(open(file_name,RELATIVE_PATH) != NO_ERROR){
                    int flag = SIG_DEBUG >= DEBUG_DETAIL;
                    CONSOLE_LOG(0,TIME_FLAG,flag, "(NEDB)File Damaged : '%s'\n", file_name.c_str());
                }
                num ++;
            }
        }
        closedir(dp);
        return NO_ERROR;
    }
    catch(NEexception& e){
        return e;
    }
}

int DataBase::open(string name,int mode){
    try{
        /* Thread Lock Check */
        if(!StatusCheck(Status(), SIG_FREE, SIG_CHECK_TIMES)){
            throw ACTION_BUSY;
        }
        int flag = SIG_DEBUG >= DEBUG_SIMPLE;
        if(mode == RELATIVE_PATH){
            CONSOLE_LOG(0,TIME_FLAG,flag, "(NEDB)Mounting '%s'\n", (__SrcDir__+name).c_str());
        }else{
            CONSOLE_LOG(0,TIME_FLAG,flag, "(NEDB)Mounting '%s'\n", name.c_str());
        }
        /* Read File */
        Memorizer RAM(NULL);
        Table* table = NULL;
        if(__Cursor__ >= MAX_TABLES){
            throw TABLE_NUM_REACH_LIMIT;
        }
        SetStatus(SIG_BLOCK);
        table = RAM.TableLoad(this, name,mode);
        if(getTable(name) == NULL){
            __Tables__[__Cursor__] = table;
            ++__Cursor__;
            SetStatus(SIG_FREE);
        }
        SetStatus(SIG_FREE);
        return NO_ERROR;
    }
    catch(NEexception& e){
        SetStatus(SIG_FREE);
        return e;
    }
}


int DataBase::exec(string sql, int& num, string& res){
    if(sql.length() == 0 || sql[sql.length() - 1] != ';'){
        return SQL_FORM_ERROR;
    }
    /////
    Parser p;
    Executor e(&p, this);
    int sql_num;
    res = "";
    string* sqls = Split(sql, ';', sql_num);
    int t = 0;
    try{
        for(int i = 0; i < sql_num - 1; i++){
            p.i_analyse(sqls[i]);
            e.execute_operation();
            string temp = e.returnValue_;
            if(temp.length() != 0){
                if(t == 0){
                    res = temp;
                    t ++;
                }
                else res = res + "\n" + temp;
            }
            p.flush();
            e.returnValue_ = "";
        }
        num = e.count_;
        return NO_ERROR;
    }
    catch(NEexception& E){
        return E;
    }
}

int DataBase::create_table(string table, string value){
    Parser p;
    Executor e(&p, this);
    p.setObject(table);
    p.setValue(value);
    p.setOperation(CREATE_TABLE);
    try{
        e.execute_operation();
        return NO_ERROR;
    }
    catch(NEexception& E){
        return E;
    }


}

int DataBase::insert_into(string table, string field, string value){
    Parser p;
    Executor e(&p, this);
    p.setObject(table);
    p.setCondition(field);
    p.setValue(value);
    p.setOperation(INSERT_VALUES);
    try{
        e.execute_operation();
        return NO_ERROR;
    }
    catch(NEexception& E){
        return E;
    }

}

int DataBase::delete_from(string table, string condition, int& num){
    Parser p;
    Executor e(&p, this);
    p.setObject(table);
    p.setCondition(condition);
    p.setOperation(DELETE_VALUES);
    try{
        e.execute_operation();
        num = e.count_;
        return NO_ERROR;
    }
    catch(NEexception& E){
        num = e.count_;
        return E;
    }

}

int DataBase::update_set(string table, string set_val, string condition, int& num){
    Parser p;
    Executor e(&p, this);
    p.setObject(table);
    p.setValue(set_val);
    p.setCondition(condition);
    p.setOperation(UPDATE_VALUES);
    try{
        e.execute_operation();
        num = e.count_;
        return NO_ERROR;
    }
    catch(NEexception& E){
        num = e.count_;
        return E;
    }

}

int DataBase::select_from(string table, string field, string condition, int& num, string& res){
    Parser p;
    Executor e(&p, this);
    p.setObject(table);
    p.setValue(field);
    p.setCondition(condition);
    p.setOperation(SELECT_VALUES);
    try{
        e.execute_operation();
        num = e.count_;
        res = e.returnValue_;
        return NO_ERROR;
    }
    catch(NEexception& E){
        num = e.count_;
        res = e.returnValue_;
        return E;
    }
}

int DataBase::describe_table(string table, string& res){
    Parser p;
    Executor e(&p, this);
    p.setObject(table);
    p.setOperation(DESCRIBE_TABLE);
    try{
        e.execute_operation();
        res = e.returnValue_;
        return NO_ERROR;
    }
    catch(NEexception& E){
        res = e.returnValue_;
        return E;
    }
}

int DataBase::select_tables(int& num, string& res){
    Parser p;
    Executor e(&p, this);
    p.setOperation(SELECT_VALUES);
    try{
        e.execute_operation();
        num = e.count_;
        res = e.returnValue_;
        return NO_ERROR;
    }
    catch(NEexception& E){
        num = e.count_;
        res = e.returnValue_;
        return E;
    }
}


int DataBase::close(){
    try{
        CONSOLE_LOG(0,TIME_FLAG,1, "(NEDB)Closing...\n");
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
        return NO_ERROR;
    }
    catch(exception& e){
        SetStatus(SIG_FREE);
        return SYSTEM_ERROR;
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


