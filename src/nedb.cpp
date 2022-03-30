#include<nedb.h>
#include<basic.h>
#include<process.h>
#include<implement.h>

typedef struct nedb{
    Table **__Tables__;
    int __Cursor__;
    nedb(){
        __Tables__ = new Table * [MAX_TABLES];
        __Cursor__ = 0;
    }
}nedb;

int nedb_auto_start(nedb **ne, char *msg){
    try{
        if(*ne == NULL){
            *ne = new nedb();
        }
        __DATABASE_INIT__();
        __LOAD_ALL__((*ne)->__Tables__, (*ne)->__Cursor__);
        msg = new char[5];
        strcpy(msg, "DONE");
        return 1;
    }
    catch(NEexception &e){
        string str = NEexceptionName[e];
        msg = new char[str.length() + 1]{{0}};
        strcpy(msg, str.c_str());
        return 0;
    }
}

int nedb_open(nedb **ne,const char *name, char **msg){
    try{
        if(*ne == NULL){
            *ne = new nedb();
        }
        __DATABASE_INIT__();
        __LOAD_FILE__((*ne)->__Tables__, (*ne)->__Cursor__, name);
        *msg = new char[5];
        strcpy(*msg,"DONE");
        return 1;
    }
    catch(NEexception &e){
        string str = NEexceptionName[e];
        *msg = new char[str.length()+10];
        strcpy(*msg, str.c_str());
        return 0;
    }
}

int nedb_exec(nedb *ne, const char *sql, char **data, char **msg){
    Parser p;
    Executor e(&p);
    int sql_num;
    string res = *new string("");
    string *sqls = Split(sql, ';', sql_num);
    int t = 0;
    try{
        for(int i = 0; i < sql_num-1; i++){
            p.i_analyse(sqls[i]);
            string temp;
            e.execute_operation(ne->__Tables__, ne->__Cursor__, temp);
            if(temp.length()!=0){
                if(t == 0){
                    res = temp;
                    t ++;
                }else res = res + "\n" + temp;
            }
        }
        *msg = new char[5];
        strcpy(*msg, "DONE");
        *data = new char[res.length() + 10];
        strcpy(*data, res.c_str());
        return 1;
    }
    catch(NEexception &e){
        string str = NEexceptionName[e];
        *msg = new char[str.length() + 1];
        strcpy(*msg, str.c_str());
        //result copy
        *data = new char[res.length() + 10];
        strcpy(*data, res.c_str());
        return 0;
    }
}


int nedb_close(nedb *ne){
    for(int i = 0; i < ne->__Cursor__; i++){
        ne->__Tables__[i] = NULL;
    }
    return 1;
}


