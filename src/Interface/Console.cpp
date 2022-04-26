#include<main.h>
#include<Basic/process.h>
#include<Utils/implement.h>
#include<Basic/data.h>


////////////Console/////////////////////////////////////////////
int ConsoleLog(int returnVal, int level, std::string content,...){
    /* Time Print */
    if(SIG_DEBUG == DEBUG_DETAIL){
        time_t timer;
        time(&timer);
        char * str = ctime(&timer);
        str[strlen(str)-1]='\0';
        printf("[%s CST] ", str);
    }
    /* Log Print */
    if(SIG_DEBUG >= level){
        va_list args;
        va_start(args,content);
        vprintf(content.c_str(),args);
        va_end(args);
    }
    /* Return Value */
    return returnVal;
}


////////////////////////////////////////////////////////////////

void __START__(){
    __MESSAGE__();
    DataBase db(__DefaultDir__);
    InputStream i;
    Parser p;
    Executor e(&p, &db);
    while(true){
        ConsoleLog(0,DEBUG_NONE,"NEDB > ");
        try{
            p.analyse(i.read());
            if(p.getCommand() == __OPERATION){
                e.execute_operation();
                if(p.getOperate() == SELECT_VALUES ||
                    p.getOperate() == DESCRIBE_TABLE ||
                    p.getOperate() == SELECT_TABLES){
                    ConsoleLog(0, DEBUG_NONE, "%s\n",db.getReturnVal().c_str());
                }
            }
            else if(p.getCommand() == __EXIT){
                db.close();
                ConsoleLog(0,DEBUG_NONE,"~\n");
                return;
            }
            else{
                e.execute_command();
            }
            ConsoleLog(0, DEBUG_NONE, "%s\n",NEexceptionName[db.getErrCode()].c_str());
        }
        catch(NEexception& e){
            ConsoleLog(0, DEBUG_NONE, "%s\n",NEexceptionName[e].c_str());
        }
        i.clear_input();
        p.flush();
    }
}
////////////////////////////////////////////////////////////////
int __DEBUG_SET__(std::string level){
    if(level == "none"){
        SIG_DEBUG = DEBUG_NONE;
    }else if(level == "simple"){
        SIG_DEBUG = DEBUG_SIMPLE;
    }else if(level == "detail"){
        SIG_DEBUG = DEBUG_DETAIL;
    }else{
        cout<<"no such level '"<<level<<"'"<<endl;
        return 0;
    }
    return 1;
}
////////////////////////////////////////////////////////////////
void __MESSAGE__(){
    cout << "Welcome to NEDB terminal. Command end with ';'." << endl;
    cout << "Server version: 22.4.26 <Stable>" << endl;
    cout << "Default resource-dir: " << __DefaultDir__ << endl;
    cout << "Enter '.help' for viewing help infomation.\n" << endl;
}

////////////////////////////////////////////////////////////////

#define __HELP_MSG__ \
    "NEDB version: 22.4.26 <Stable>\n\
     Data Type Support >\n\
    \t[  int     ]  ->  int\n\
    \t[  int     ]  ->  int\n\
    \t[  int64   ]  ->  long int\n\
    \t[  real    ]  ->  double\n\
    \t[  text    ]  ->  char[32]\n\
    \t[ longtext ]  ->  char[255]\n\
    \n\
    Command Insturction >\n\
    \n\
    \t[EXIT] .exit\n\
    \n\
    \t[HELP] .help\n\
    \n\
    \t[GET DIR] .dir\n\
    \t * Default dir : '/home/jianxf/.nesrc/'.\n\
    \n\
    \t[SET DIR] .setdir 'full_dir'\n\
    \t * Use '-d' replacing 'full_dif' to return to default.\n\
    \n\
    \t[DIR INIT] .dirinit\n\
    \t * Automatically check and create current dir.\n\
    \n\
    \t[OPEN FILE] .open 'table_name'\n\
    \t * Enter table name without suffix.\n\
    \n\
    \t[OPEN ALL] .openall\n\
    \t * Open all table from current dir.\n\
    \n\
    \t[GET SIZE] .size\n\
    \t * Default size : 400 with unit 'Byte'.\n\
    \n\
    \t[SET PAGE SIZE] .setsize 'size'.\n\
    \t * Size range : 100 - 4000 with unit 'Byte'.\n\
    \t * Use '-d' replacing 'size' to return to default.\n\
    \n\
    \t[TABLE CREATE] create table 'table_name' ('data_title' 'data_type', ... );\n\
    \t * The first parm will be set as PRIMARY KEY by default.\n\
    \t * Add 'key' after a parm to designate.\n\
    \t * Parm typed 'longtext' is not allowed to be set as the PRIMARY KEY.\n\
    \n\
    \t[TABLE REMOVE] drop table 'table_name';\n\
    \n\
    \t[DATA INSERT] insert into 'table_name' ('parm_name', ...) values ('parm_value', ...);\n\
    \n\
    \t[DATA SELECT] select 'parm_name' from 'table_name' where 'conditions';\n\
    \n\
    \t[DATA DELETE] delete from 'table_name' where 'conditions';\n\
    \n\
    \t[DATA UPDATE] update 'table_name' set 'parm_name' = 'parm_value', ... where 'conditions';\n\
    \n\
    \t[STRUCTURE CHECK] describe table 'table_name';\n\
    \n"

void __HELP__(){
    cout << __HELP_MSG__;
}