#include<Process.h>
#include <Data.h>
//#include<Basic.h>

string const __Type__[]={"INT","INT64","REAL","TEXT","LONGTEXT"};

Table** __TABLES__;
int __CURSOR__;

void __DATABASE_INIT__(){
    __TABLES__ = new Table*[MAX_TABLES];
    __CURSOR__ = 0;
}

int __TABLE_LOCATED_BY_NAME__(string name){
    for(int i = 0; i < __CURSOR__ ; i++){
        string temp = __TABLES__[i]->getName();
        if(temp.compare(name) == 0) return i; 
    }
    return -1;
}

void __START__(){
    __DATABASE_INIT__();
    __MESSAGE__();
    InputStream i;
    Parser p;
    Executor e;
    while(true){
        printf("LiteDB > ");
        p.analyse(i.read());
        if(p.getCommand() == __OPERATE){
            e.execute(p.getOperate(), p.getStatment());
        }else if(p.getCommand() == __EXIT){
            printf("~\n");
            return;
        }else{
            e.execute(p.getCommand(), p.getStatment());
        }
        i.clear_input();
    }
}

void __MESSAGE__(){
    cout<<"Welcome to LiteDB terminal. Command end with ';'."<<endl;
    cout<<"Server version: 22.3.21 <Preview>"<<endl;
    cout<<"Enter '.help' for viewing help infomation.\n"<<endl;
}

void __HELP__(){
    cout<<"LiteDB version: 22.3.21 <Preview>"<<endl;
    cout<<" "<<endl;
    cout<<"Data Type Support >"<<endl;
    cout<<" "<<endl;
    cout<<"\t[  INT     ]  ->  int"<<endl;
    cout<<"\t[  INT64   ]  ->  long int"<<endl;
    cout<<"\t[  REAL    ]  ->  double"<<endl;
    cout<<"\t[  TEXT    ]  ->  varchar(32)"<<endl;
    cout<<"\t[ LONGTEXT ]  ->  varchar(255)"<<endl;
    cout<<" "<<endl;
    cout<<"Command Insturction >"<<endl;
    cout<<" "<<endl;
    cout<<"\t[EXIT] .exit"<<endl;
    cout<<"   "<<endl;
    cout<<"\t[HELP] .help"<<endl;
    cout<<"   "<<endl;
    cout<<"\t[TABLE CREATE] CREATE table_name(data_title : data_type, ... );"<<endl;
    cout<<"\t * The first parm will be set as PRIMARY KEY by default."<<endl;
    cout<<"\t * Add 'KEY(key_name)' as a parm to designate."<<endl;
    cout<<"   "<<endl;
    cout<<"\t[TABLE REMOVE] REMOVE table_name;"<<endl;
    cout<<"   "<<endl;
    cout<<"\t[STRUCTURE CHECK] DESCRIBE table_name;"<<endl;
    cout<<"   "<<endl;
    cout<<"\t[PAGE REMOVE] REMOVE table_name(page_index);"<<endl;
    cout<<"   "<<endl;
    cout<<"\t[DATA INSERT] INSERT table_name(data_01, ...);"<<endl;
    cout<<"   "<<endl;
    cout<<"\t[DATA DELETE] DELETE table_name(data_index);"<<endl;
    cout<<"   \n"<<endl;
}