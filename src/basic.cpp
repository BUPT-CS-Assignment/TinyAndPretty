#include<Process.h>
#include<Data.h>
#include<Storage.h>

////////////////////////////////////////////////////////////////
string kHomeDir = "/home/jianxf/LiteSrc/Tables/";
////////////////////////////////////////////////////////////////
string const kTypeName[]={"INT","INT64","REAL","TEXT","LONGTEXT"};
int const kTypeSize []={4,8,8,32,255};
////////////////////////////////////////////////////////////////
string kFramSuffix = ".ldf";
string kDataSuffix = ".ldd";
string kIndexSuffix = ".ldi";
////////////////////////////////////////////////////////////////
Table** __TABLES__;
int __CURSOR__;
////////////////////////////////////////////////////////////////
void __DATABASE_INIT__(){
    __TABLES__ = new Table*[MAX_TABLES];
    __CURSOR__ = 0;
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
    SELECT * FROM table_name LIMIT 3;
    SELECT * FROM table_name 

    UPDATE table_name SET  NAME = name  WHERE ID = 1 ;
    UPDATE table_name SET NAME = name , SCHOOL = school 

    DELETE FROM table_name WHERE ID = 1 LIMIT 3;
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
    __DATABASE_INIT__();
    __MESSAGE__();
    InputStream i;
    Parser p;
    Executor e(&p);
    while(true){
        printf("LiteDB > ");
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

////////////////////////////////////////////////////////////////
void __LOAD_FILE__(string fileName){
    Memorizer RAM;
    Table* table = NULL;
    if((table = RAM.TableLoad(fileName))==NULL){
        cout<<"Load Table '"<<fileName<<"' Failed."<<endl;
        return;
    }
    __TABLES__[__CURSOR__ ++ ] = table;
    cout<<"Load Table '"<<fileName<<"' Success."<<endl;
}

////////////////////////////////////////////////////////////////
void __MESSAGE__(){
    cout<<"Welcome to LiteDB terminal. Command end with ';'."<<endl;
    cout<<"Server version: 22.3.21 <Preview>"<<endl;
    cout<<"Enter '.help' for viewing help infomation.\n"<<endl;
}

////////////////////////////////////////////////////////////////
void __HELP__(){
    cout<<"LiteDB version: 22.3.21 <Preview>"<<endl;
    cout<<" "<<endl;
    cout<<"Data Type Support >"<<endl;
    cout<<" "<<endl;
    cout<<"\table_ptr_[  INT     ]  ->  int"<<endl;
    cout<<"\table_ptr_[  INT64   ]  ->  long int"<<endl;
    cout<<"\table_ptr_[  REAL    ]  ->  double"<<endl;
    cout<<"\table_ptr_[  TEXT    ]  ->  varchar(32)"<<endl;
    cout<<"\table_ptr_[ LONGTEXT ]  ->  varchar(255)"<<endl;
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
    cout<<"\table_ptr_[TABLE CREATE] CREATE table_name(data_title : data_type, ... );"<<endl;
    cout<<"\table_ptr_ * The first parm will be set as PRIMARY KEY by default."<<endl;
    cout<<"\table_ptr_ * Add 'KEY(key_name)' as a parm to designate."<<endl;
    cout<<"\table_ptr_ * Type 'LONGTEXT' is not allowed to be set as the PRIMARY KEY"<<endl;
    cout<<"   "<<endl;
    cout<<"\table_ptr_[TABLE REMOVE] REMOVE table_name;"<<endl;
    cout<<"   "<<endl;
    cout<<"\table_ptr_[STRUCTURE CHECK] DESCRIBE table_name;"<<endl;
    cout<<"   "<<endl;
    cout<<"\table_ptr_[PAGE REMOVE] REMOVE table_name(page_index);"<<endl;
    cout<<"   "<<endl;
    cout<<"\table_ptr_[DATA INSERT] INSERT table_name(data_01, ...);"<<endl;
    cout<<"   "<<endl;
    cout<<"\table_ptr_[DATA DELETE] DELETE table_name(data_index);"<<endl;
    cout<<"   \n"<<endl;
}