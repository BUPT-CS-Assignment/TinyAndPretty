#include<basic.h>
#include<process.h>
#include<implement.h>
#include<data.h>

NEDB::NEDB(string dir){
    if(dir.length() == 0 || dir[dir.length() - 1] != '/'){
        dir = dir + "/";
    }
    __SrcDir__ = dir;
    __Tables__ = new Table * [MAX_TABLES];
    __PageSize__ = DEFAULT_PAGE_SIZE;
    __Cursor__ = 0;
    __Msg__ = "";
    __Data__ = "";
}

int NEDB::setDir(string dir){
    if(dir.length() == 0){
        __Msg__ = "dir error";
        return 0;
    }
    if(dir[dir.length() - 1] != '/'){
        dir = dir + "/";
    }
    __SrcDir__ = dir;
    __Msg__ = "complete";
    return 1;
}

string NEDB::getDir(){
    return __SrcDir__;
}

string NEDB::getData(){
    return __Data__;
}

string NEDB::getMsg(){
    return __Msg__;
}

int NEDB::getDefaultPageSize(){
    return __PageSize__;
}

void NEDB::setMsg(string msg){
    __Msg__ = msg;
}

Table *NEDB::getTable(int i){
    if(i >= __Cursor__) return NULL;
    return __Tables__[i];
}

int NEDB::getCursor(){
    return __Cursor__;
}

void NEDB::setData(string data){
    __Data__ = data;
}



int NEDB::setDefaultPageSize(int size){
    if(size < 100 || size > 4000){
        __Msg__ = "size not allowed";
        return 0;
    }
    __PageSize__ = size;
    __Msg__ = "complete";
    return 1;
}

void NEDB::addTable(Table *table){
    __Tables__[__Cursor__] = table;
    __Cursor__ ++;
}

void NEDB::dropTable(string name){
    try{
        for(int i = 0; i < __Cursor__; i++){
            if(__Tables__[i] == NULL){
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
                return;
            }
        }
        throw TABLE_NOT_FOUND;
    }
    catch(NEexception &e){
        throw e;
    }
}

int NEDB::dirInit(){
    try{
        int dir_num = 0;
        string *dirs = Split(__SrcDir__, '/', dir_num);
        string dir = "";
        for(int i = 1; i < dir_num - 1; i++){
            dir = dir + "/" + dirs[i];
            if(NULL == opendir(dir.c_str())){
                if(mkdir(dir.c_str(), S_IRWXU) == -1){
                    throw DIR_ERROR;
                }
            }
        }
        __Msg__ = "complete";
        return 1;
    }
    catch(NEexception &e){
        __Msg__ = NEexceptionName[e];
        return 0;
    }
}

int NEDB::scan(){
    try{
        DIR *dp = opendir(__SrcDir__.c_str());
        if(dp == NULL){
            throw DIR_ERROR;
        }
        regex layout(".+\\.nef");
        struct dirent *dirfiles;
        while((dirfiles = readdir(dp)) != NULL){
            if(__Cursor__ >= MAX_TABLES){
                throw TABLE_NUM_REACH_LIMIT;
            }
            string file_name = dirfiles->d_name;
            if(regex_match(file_name, layout)){
                file_name = file_name.substr(0, file_name.find("."));
                if(open(file_name)){
                    return 0;
                }
            }
        }
        closedir(dp);
        __Msg__ = "complete";
        return 1;
    }
    catch(NEexception &e){
        __Msg__ = NEexceptionName[e];
        return 0;
    }
}

int NEDB::open(string name){
    try{
        Memorizer RAM(NULL);
        Table *table = NULL;
        if(__Cursor__ >= MAX_TABLES){
            throw TABLE_NUM_REACH_LIMIT;
        }
        table = RAM.TableLoad(this, name);
        __Tables__[__Cursor__] = table;
        ++__Cursor__;
        __Msg__ = "complete";
        return 1;
    }
    catch(NEexception &e){
        __Msg__ = NEexceptionName[e];
        return 0;
    }
}

int NEDB::exec(string sql){
    if(sql.length() == 0 || sql[sql.length()-1]!=';'){
        __Msg__ = NEexceptionName[SQL_FORM_ERROR];
        return 0;
    }
    /////
    Parser p;
    Executor e(&p, this);
    int sql_num;
    setData("");
    string res = "";
    string *sqls = Split(sql, ';', sql_num);
    int t = 0;
    try{
        for(int i = 0; i < sql_num - 1; i++){
            p.i_analyse(sqls[i]);
            e.execute_operation();
            string temp = getData();
            if(temp.length() != 0){
                if(t == 0){
                    res = temp;
                    t ++;
                }
                else res = res + "\n" + temp;
            }
            setData("");
            p.flush();
        }
        __Data__ = res;
        __Msg__ = "complete";
        return 1;
    }
    catch(NEexception &E){
        __Data__ = res;
        __Msg__ = NEexceptionName[E];
        return 0;
    }
}

int NEDB::close(){
    try{
        for(int i = 0; i < __Cursor__; i++){
            __Tables__[i]->Erase();
        }
        delete[] __Tables__;
        __Msg__ = "complete";
        return 1;
    }
    catch(exception &e){
        __Msg__ = "system error";
        return 0;
    }
}

Table *NEDB::getTable(string name){
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
    catch(NEexception &e){
        throw e;
    }
}

////////////////////////////////////////////////////////////////

void __START__(){
    __MESSAGE__();
    NEDB db(__DefaultDir__);
    InputStream i;
    Parser p;
    Executor e(&p, &db);
    while(true){
        printf("NEDB > ");
        try{
            p.analyse(i.read());
            if(p.getCommand() == __OPERATION){
                e.execute_operation();
                if(p.getOperate() == SELECT_VALUES || p.getOperate() == DESCRIBE_TABLE
                    || p.getOperate() == SELECT_TABLES){
                    cout << db.getData() << endl;
                }
            }
            else if(p.getCommand() == __EXIT){
                db.close();
                printf("~\n");
                return;
            }
            else{
                e.execute_command();
            }
            cout << "[complete]" << endl;
        }
        catch(NEexception &e){
            cout << "[error] " << NEexceptionName[e] << endl;
        }
        i.clear_input();
        p.flush();
    }
}

////////////////////////////////////////////////////////////////
void __MESSAGE__(){
    cout << "Welcome to NEDB terminal. Command end with ';'." << endl;
    cout << "Server version: 22.4.17 <Stable>" << endl;
    cout << "Default resource-dir: " << __DefaultDir__ << endl;
    cout << "Enter '.help' for viewing help infomation.\n" << endl;
}

////////////////////////////////////////////////////////////////
void __HELP__(){
    cout << "NEDB version: 22.4.17 <Stable>" << endl;
    cout << " " << endl;
    cout << "Data Type Support >" << endl;
    cout << " " << endl;
    cout << "\t[  int     ]  ->  int" << endl;
    cout << "\t[  int64   ]  ->  long int" << endl;
    cout << "\t[  real    ]  ->  double" << endl;
    cout << "\t[  text    ]  ->  char[32]" << endl;
    cout << "\t[ longtext ]  ->  char[255]" << endl;
    cout << " " << endl;
    cout << "Command Insturction >" << endl;
    cout << " " << endl;
    cout << "\t[EXIT] .exit" << endl;
    cout << "   " << endl;
    cout << "\t[HELP] .help" << endl;
    cout << "   " << endl;
    cout << "\t[GET DIR] .dir" << endl;
    cout << "\t * Default dir : '/home/jianxf/.nesrc/'." << endl;
    cout << "   " << endl;
    cout << "\t[SET DIR] .setdir 'full_dir'" << endl;
    cout << "\t * Use '-d' replacing 'full_dif' to return to default." << endl;
    cout << "   " << endl;
    cout << "\t[DIR INIT] .dirinit" << endl;
    cout << "\t * Automatically check and create current dir." << endl;
    cout << "   " << endl;
    cout << "\t[OPEN FILE] .open 'table_name'" << endl;
    cout << "\t * Enter table name without suffix." << endl;
    cout << "   " << endl;
    cout << "\t[OPEN ALL] .openall" << endl;
    cout << "\t * Open all table from current dir." << endl;
    cout << "   " << endl;
    cout << "\t[GET SIZE] .size" << endl;
    cout << "\t * Default size : 400 with unit 'Byte'." << endl;
    cout << "   " << endl;
    cout << "\t[SET PAGE SIZE] .setsize 'size'." << endl;
    cout << "\t * Size range : 100 - 4000 with unit 'Byte'." << endl;
    cout << "\t * Use '-d' replacing 'size' to return to default." << endl;
    cout << "   " << endl;
    cout << "\t[TABLE CREATE] create table 'table_name' ('data_title' 'data_type', ... );" << endl;
    cout << "\t * The first parm will be set as PRIMARY KEY by default." << endl;
    cout << "\t * Add 'key' after a parm to designate." << endl;
    cout << "\t * Parm typed 'longtext' is not allowed to be set as the PRIMARY KEY." << endl;
    cout << "   " << endl;
    cout << "\t[TABLE REMOVE] drop table 'table_name';" << endl;
    cout << "   " << endl;
    cout << "\t[DATA INSERT] insert into 'table_name' ('parm_name', ...) values ('parm_value', ...);" << endl;
    cout << "   " << endl;
    cout << "\t[DATA SELECT] select 'parm_name' from 'table_name' where 'conditions';" << endl;
    cout << "   " << endl;
    cout << "\t[DATA DELETE] delete from 'table_name' where 'conditions';" << endl;
    cout << "   " << endl;
    cout << "\t[DATA UPDATE] update 'table_name' set 'parm_name' = 'parm_value', ... where 'conditions';" << endl;
    cout << "   " << endl;
    cout << "\t[STRUCTURE CHECK] describe table 'table_name';" << endl;
    cout << "   " << endl;
}

NEdb::NEdb(const char *dir){
    nedb = new NEDB(dir);
}

char *NEdb::getDir(){
    if(nedb == NULL) return new char[1]{{0}};
    string dir = nedb->getDir();
    char *res = new char[dir.length() + 1];
    strcpy(res, dir.c_str());
    return res;
}

int NEdb::getDefaultPageSize(){
    if(nedb == NULL) return -1;
    return nedb->getDefaultPageSize();
}

int NEdb::setDefaultPageSize(int size){
    if(nedb == NULL) return -1;
    return nedb->setDefaultPageSize(size);
}

int NEdb::setDir(const char *dir){
    if(nedb == NULL) return -1;
    return nedb->setDir(dir);
}

char *NEdb::getMsg(){
    if(nedb == NULL) return new char[16]{"Pointer error"};
    string dir = nedb->getMsg();
    char *res = new char[dir.length() + 1];
    strcpy(res, dir.c_str());
    return res;
}

char *NEdb::getData(){
    if(nedb == NULL) return new char[1]{{0}};
    string dir = nedb->getData();
    char *res = new char[dir.length() + 1];
    strcpy(res, dir.c_str());
    return res;
}

int NEdb::dirInit(){
    if(nedb == NULL) return -1;
    return nedb->dirInit();
}

int NEdb::scan(){
    if(nedb == NULL) return -1;
    return nedb->scan();
}

int NEdb::open(const char *fileName){
    if(nedb == NULL) return -1;
    return nedb->open(fileName);
}

int NEdb::exec(const char *sql){
    if(nedb == NULL) return -1;
    return nedb->exec(sql);
}

int NEdb::close(){
    if(nedb == NULL) return 1;
    int res = nedb->close();
    delete nedb;
    return res;
}