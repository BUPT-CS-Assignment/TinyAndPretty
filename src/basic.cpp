#include<Process.h>
#include<Data.h>
#include<Storage.h>
#include<Implement.h>
////////////////////////////////////////////////////////////////
string kHomeDir = "/home/jianxf/.nesrc/tables/";
////////////////////////////////////////////////////////////////
string const kTypeName[]={"int","int64","real","text","longtext"};
int const kTypeSize []={4,8,8,32,255};
////////////////////////////////////////////////////////////////
string kFramSuffix = ".nef";
string kDataSuffix = ".ned";
string kIndexSuffix = ".nei";
////////////////////////////////////////////////////////////////
Table** __TABLES__;
int __CURSOR__;
////////////////////////////////////////////////////////////////
bool __DATABASE_INIT__(){
    __TABLES__ = new Table*[MAX_TABLES];
    __CURSOR__ = 0;
    int dir_num = 0;
    string* dirs = Split(kHomeDir,'/',dir_num);
    string dir = "";
    for(int i = 1; i < dir_num-1; i++){
        dir = dir + "/" + dirs[i]; 
        if(NULL == opendir(dir.c_str())){
            if(mkdir(dir.c_str(),S_IRWXU) == -1){
                cout<<"<E> HOME DIR ERROR"<<endl;
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
    for(int i = 0; i < __CURSOR__ ; i++){
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
        }else if(p.getCommand() == __EXIT){
            printf("~\n");
            return;
        }else{
            e.execute_command();
        }
        i.clear_input();
        p.flush();
    }
}

bool __LOAD_ALL__(){
    DIR *dp = opendir(kHomeDir.c_str());
    if(dp == NULL){
        cout<<"<E> HOME DIR ERROR"<<endl;
        return false;
    }
    regex layout(".+\\.nef");
    struct dirent *dirfiles;
    while((dirfiles = readdir(dp))!=NULL){
        string fileName = dirfiles->d_name;
        if(regex_match(fileName,layout)){
            string tableName =  fileName.substr(0,fileName.find('.'));
            __LOAD_FILE__(tableName);
        }
    }
    closedir(dp);
    return true;
}

////////////////////////////////////////////////////////////////
void __LOAD_FILE__(string tableName){
    Memorizer RAM;
    Table* table = NULL;
    if((table = RAM.TableLoad(tableName))==NULL){
        cout<<"Load Table '"<<tableName<<"' Failed."<<endl;
        return;
    }
    __TABLES__[__CURSOR__ ++ ] = table;
    cout<<"Load Table '"<<tableName<<"' Success."<<endl;
}

////////////////////////////////////////////////////////////////
void __MESSAGE__(){
    cout<<"Welcome to NEDB terminal. Command end with ';'."<<endl;
    cout<<"Server version: 22.3.30 <Pre-Release>"<<endl;
    cout<<"Current file-dir: "<<kHomeDir<<endl;
    cout<<"Enter '.help' for viewing help infomation.\n"<<endl;
}

////////////////////////////////////////////////////////////////
void __HELP__(){
    cout<<"NEDB version: 22.3.30 <Pre-Release>"<<endl;
    cout<<" "<<endl;
    cout<<"Data Type Support >"<<endl;
    cout<<" "<<endl;
    cout<<"\table_ptr_[  INT     ]  ->  int"<<endl;
    cout<<"\table_ptr_[  INT64   ]  ->  long int"<<endl;
    cout<<"\table_ptr_[  REAL    ]  ->  double"<<endl;
    cout<<"\table_ptr_[  TEXT    ]  ->  char[32]"<<endl;
    cout<<"\table_ptr_[ LONGTEXT ]  ->  char[255]"<<endl;
    cout<<" "<<endl;
    cout<<"Command Insturction >"<<endl;
    cout<<" "<<endl;
    cout<<"\table_ptr_[EXIT] .exit"<<endl;
    cout<<"   "<<endl;
    cout<<"\table_ptr_[HELP] .help"<<endl;
    cout<<"   "<<endl;
    cout<<"\table_ptr_[OPEN FILE] .open table_name"<<endl;
    cout<<"\table_ptr_ * Just enter the table name, without suffix."<<endl;
    cout<<"   "<<endl;
    cout<<"\table_ptr_[TABLE CREATE] CREATE TABLE table_name(data_title data_type, ... );"<<endl;
    cout<<"\table_ptr_ * The first parm will be set as PRIMARY KEY by default."<<endl;
    cout<<"\table_ptr_ * Add 'KEY' after a parm to designate."<<endl;
    cout<<"\table_ptr_ * Type 'LONGTEXT' is not allowed to be set as the PRIMARY KEY"<<endl;
    cout<<"   "<<endl;
    cout<<"\table_ptr_[TABLE REMOVE] DROP TABLE table_name;"<<endl;
    cout<<"   "<<endl;
    cout<<"\table_ptr_[DATA INSERT] INSERT INTO table_name (parm_name, ...) VALUES (parm_value, ...);"<<endl;
    cout<<"   "<<endl;
    cout<<"\table_ptr_[DATA SELECT] SELECT parm_name FROM table_name WHERE condition;"<<endl;
    cout<<"   "<<endl;
    cout<<"\table_ptr_[DATA DELETE] DELETE FROM table_name WHERE condition;"<<endl;
    cout<<"   \n"<<endl;
    cout<<"\table_ptr_[DATA UPDATE] UPDATE table_name SET parm_name = parm_value, ... WHERE condition;"<<endl;
    cout<<"   \n"<<endl;
    cout<<"\table_ptr_[STRUCTURE CHECK] DESCRIBE TABLE table_name;"<<endl;
    cout<<"   "<<endl;
}