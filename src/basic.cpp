#include<process.h>
#include<data.h>
#include<storage.h>
#include<implement.h>
////////////////////////////////////////////////////////////////
//string __HomeDir__ = "/home/jianxf/.nesrc/tables/";
////////////////////////////////////////////////////////////////
string const kTypeName[] = {"int","int64","real","text","longtext"};
int const kTypeSize[] = {4,8,8,32,255};
////////////////////////////////////////////////////////////////
string __FramSuffix__ = ".nef";
string __DataSuffix__ = ".ned";
string __IndexSuffix__ = ".nei";
////////////////////////////////////////////////////////////////
Table **__TABLES__;
int __CURSOR__;
////////////////////////////////////////////////////////////////
string const NEexceptionName[] = {
    //system
    "system error",
    //dir error
    "home dir error",
    //file error
    "file open error", "file not found", "file damaged",
    //sql error
    "sql form mismatch", "sql undefined",
    //table error
    "table existed", "table not found",
    //parm error
    "param empty", "param form mismatch", "param not found", "param num mismatch",
    "value empty",
    //nedb type error
    "type undefined",
    "type int mismatch", "type int64 mismatch", "type real mismatch",
    "type int overflow", "type int64 overflow", "type real overflow",
    "type text overflow", "type longtext overflow",
    //prim key error
    "key type not allowed",
    "key value existed", "key value required", "key value change not allowed",
    //data error
    "data not found",
    //nedb operator charactor error
    "compare nonsupport",
};


////////////////////////////////////////////////////////////////
void __START__(){
    __MESSAGE__();
    try{
        __DATABASE_INIT__();
    }
    catch(NEexception &e){
        cout << "[Error] " << NEexceptionName[e] << endl;
        return;
    }
    __TABLES__ = new Table * [MAX_TABLES];
    __CURSOR__ = 0;
    string res = *new string("");
    InputStream i;
    Parser p;
    Executor e(&p);
    while(true){
        printf("nedb > ");
        try{
            p.analyse(i.read());
            if(p.getCommand() == __OPERATION){
                e.execute_operation(__TABLES__, __CURSOR__, res);
                if(p.getOperate() == SELECT_VALUES || p.getOperate() == DESCRIBE_TABLE
                    || p.getOperate() == SELECT_TABLES){
                    cout << res << endl;
                }
            }
            else if(p.getCommand() == __EXIT){
                printf("~\n");
                return;
            }
            else{
                e.execute_command();
            }
        }
        catch(NEexception &e){
            cout << "[Error] " << NEexceptionName[e] << endl;
        }
        i.clear_input();
        p.flush();
    }
}

void __DATABASE_INIT__(){
    try{
        int dir_num = 0;
        string *dirs = Split(__HomeDir__, '/', dir_num);
        string dir = "";
        for(int i = 1; i < dir_num - 1; i++){
            dir = dir + "/" + dirs[i];
            if(NULL == opendir(dir.c_str())){
                if(mkdir(dir.c_str(), S_IRWXU) == -1){
                    throw HOME_DIR_ERROR;
                }
            }
        }
    }
    catch(NEexception &e){
        throw e;
    }
}

int __TABLE_LOCATED_BY_NAME__(Table **table, int cursor, string name){
    for(int i = 0; i < cursor; i++){
        string temp = table[i]->getName();
        if(temp.compare(name) == 0) return i;
    }
    return -1;
}

void __LOAD_ALL__(Table **tables, int &cursor){
    try{
        DIR *dp = opendir(__HomeDir__);
        if(dp == NULL){
            throw HOME_DIR_ERROR;
        }
        regex layout(".+\\.nef");
        struct dirent *dirfiles;
        while((dirfiles = readdir(dp)) != NULL){
            string fileName = dirfiles->d_name;
            if(regex_match(fileName, layout)){
                string tableName = fileName.substr(0, fileName.find('.'));
                __LOAD_FILE__(tables, cursor, tableName);
            }
        }
        closedir(dp);
    }
    catch(NEexception &e){
        throw e;
    }
}

////////////////////////////////////////////////////////////////
void __LOAD_FILE__(Table **tables, int &cursor, string tableName){
    try{
        Memorizer RAM;
        Table *table = NULL;
        table = RAM.TableLoad(tableName);
        tables[cursor] = table;
        ++cursor;
    }
    catch(NEexception &e){
        throw e;
    }
}

////////////////////////////////////////////////////////////////
void __MESSAGE__(){
    cout << "Welcome to NEDB terminal. Command end with ';'." << endl;
    cout << "Server version: 22.3.31 <Stable>" << endl;
    cout << "Current file-dir: " << __HomeDir__ << endl;
    cout << "Enter '.help' for viewing help infomation.\n" << endl;
}

////////////////////////////////////////////////////////////////
void __HELP__(){
    cout << "NEDB version: 22.3.31 <Stable>" << endl;
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
    cout << "\t[OPEN FILE] .open 'table_name'" << endl;
    cout << "\t * Just enter the table name, without suffix." << endl;
    cout << "   " << endl;
    cout << "\t[OPEN ALL] .openall" << endl;
    cout << "   " << endl;
    cout << "\t[TABLE CREATE] create table 'table_name' ('data_title' 'data_type', ... );" << endl;
    cout << "\t * The first parm will be set as PRIMARY KEY by default." << endl;
    cout << "\t * Add 'key' after a parm to designate." << endl;
    cout << "\t * Parm typed 'longtext' is not allowed to be set as the PRIMARY KEY" << endl;
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