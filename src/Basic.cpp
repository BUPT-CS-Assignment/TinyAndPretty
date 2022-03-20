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
    cout<<"----------------------------------------------"<<endl;
    cout<<" Data Type Support :"<<endl;
    cout<<" | INT"<<endl;
    cout<<" | INT64"<<endl;
    cout<<" | REAL"<<endl;
    cout<<" | TEXT"<<endl;
    cout<<" | LONGTEXT"<<endl;
    cout<<"----------------------------------------------"<<endl;
    cout<<" Command Info :"<<endl;
    cout<<" --EXIT--"<<endl;
    cout<<" | .exit"<<endl;
    cout<<" --HELP--"<<endl;
    cout<<" | .help"<<endl;
    cout<<" --TABLE CREATE--"<<endl;
    cout<<" | CREATE table_name(data_title : data_type, ... ,KEY(key_name));"<<endl;
    cout<<" --TABLE REMOVE--"<<endl;
    cout<<" | REMOVE table_name;"<<endl;
    cout<<" --INSERT DATA--"<<endl;
    cout<<" | INSERT table_name(data_01, ...);"<<endl;
    cout<<" --DELETE DATA--"<<endl;
    cout<<" | DELETE table_name(data_index);"<<endl;
    cout<<"----------------------------------------------\n"<<endl;
}