#include<Process.h>
#include<Data.h>
#include<Storage.h>
#include<Implement.h>
////////////////////////////////////////////////////////////////
string kHomeDir = "/home/jianxf/.nesrc/tables/";
////////////////////////////////////////////////////////////////
string const kTypeName[] = {"int","int64","real","text","longtext"};
int const kTypeSize[] = {4,8,8,32,255};
////////////////////////////////////////////////////////////////
string kFramSuffix = ".nef";
string kDataSuffix = ".ned";
string kIndexSuffix = ".nei";
////////////////////////////////////////////////////////////////
Table **__TABLES__;
int __CURSOR__;
////////////////////////////////////////////////////////////////
bool __DATABASE_INIT__(){
    __TABLES__ = new Table * [MAX_TABLES];
    __CURSOR__ = 0;
    int dir_num = 0;
    string *dirs = Split(kHomeDir, '/', dir_num);
    string dir = "";
    for(int i = 1; i < dir_num - 1; i++){
        dir = dir + "/" + dirs[i];
        if(NULL == opendir(dir.c_str())){
            if(mkdir(dir.c_str(), S_IRWXU) == -1){
                cout << "<E> HOME DIR ERROR" << endl;
                return false;
            }
        }
    }
    return true;
}

/*
    CREATE TABLE table_name (
        ID INT KEY,
        NAME TEXT,
        SCHOOL LONGTEXT
    );

    INSERT INTO table_name ( ID , NAME ) VALUES ( 1 , name );
    INSERT INTO table_name VALUES ( 1 , name , school );

    SELECT ID , INT FROM table_name WHERE ID = 1 , NAME = name;
    SELECT * FROM table_name

    UPDATE table_name SET  NAME = name  WHERE ID = 1 ;
    UPDATE table_name SET NAME = name , SCHOOL = school

    DELETE FROM table_name WHERE ID = 1;
    DELETE FROM table_name

*/

////////////////////////////////////////////////////////////////
int __TABLE_LOCATED_BY_NAME__(string name){
    for(int i = 0; i < __CURSOR__; i++){
        string temp = __TABLES__[i]->getName();
        if(temp.compare(name) == 0) return i;
    }
    return -1;
}


////////////////////////////////////////////////////////////////
void __START__(){
    __MESSAGE__();
    if(!__DATABASE_INIT__()) return;
    InputStream i;
    Parser p;
    Executor e(&p);
    while(true){
        printf("nedb > ");
        p.analyse(i.read());
        if(p.getCommand() == __OPERATION){
            e.execute_operation();
        }
        else if(p.getCommand() == __EXIT){
            printf("~\n");
            return;
        }
        else{
            e.execute_command();
        }
        i.clear_input();
        p.flush();
    }
}

bool __LOAD_ALL__(){
    DIR *dp = opendir(kHomeDir.c_str());
    if(dp == NULL){
        cout << "<E> HOME DIR ERROR" << endl;
        return false;
    }
    regex layout(".+\\.nef");
    struct dirent *dirfiles;
    while((dirfiles = readdir(dp)) != NULL){
        string fileName = dirfiles->d_name;
        if(regex_match(fileName, layout)){
            string tableName = fileName.substr(0, fileName.find('.'));
            __LOAD_FILE__(tableName);
        }
    }
    closedir(dp);
    return true;
}

////////////////////////////////////////////////////////////////
void __LOAD_FILE__(string tableName){
    Memorizer RAM;
    Table *table = NULL;
    if((table = RAM.TableLoad(tableName)) == NULL){
        cout << "Load Table '" << tableName << "' Failed." << endl;
        return;
    }
    __TABLES__[__CURSOR__ ++] = table;
    cout << "Load Table '" << tableName << "' Success." << endl;
}

////////////////////////////////////////////////////////////////
void __MESSAGE__(){
    cout << "Welcome to NEDB terminal. Command end with ';'." << endl;
    cout << "Server version: 22.3.30 <Pre-Release>" << endl;
    cout << "Current file-dir: " << kHomeDir << endl;
    cout << "Enter '.help' for viewing help infomation.\n" << endl;
}

////////////////////////////////////////////////////////////////
void __HELP__(){
    cout << "NEDB version: 22.3.30 <Pre-Release>" << endl;
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
    cout << "\t[OPEN FILE] .open table_name" << endl;
    cout << "\t * Just enter the table name, without suffix." << endl;
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