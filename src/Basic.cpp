#include<Process.h>
#include<Data/Table.h>
//#include<Basic.h>

string const __Type__[]={
    "short","int","uint",
    "char","varchar11","varchar32","varchar255"
    };


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