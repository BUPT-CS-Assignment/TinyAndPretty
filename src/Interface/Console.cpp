#include<main.h>
#include<Basic/process.h>
#include<Utils/implement.h>
#include<Basic/data.h>

////////////////////////////////////////////////////////////////

void __START__(){
    __MESSAGE__();
    DataBase db(__DefaultDir__);
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
                    cout << db.getReturnVal() << endl;
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
            cout << NEexceptionName[db.getErrCode()] << endl;
        }
        catch(NEexception &e){
            cout << NEexceptionName[e] << endl;
        }
        i.clear_input();
        p.flush();
    }
}

////////////////////////////////////////////////////////////////
void __MESSAGE__(){
    cout << "Welcome to NEDB terminal. Command end with ';'." << endl;
    cout << "Server version: 22.4.25 <Stable>" << endl;
    cout << "Default resource-dir: " << __DefaultDir__ << endl;
    cout << "Enter '.help' for viewing help infomation.\n" << endl;
}

////////////////////////////////////////////////////////////////
void __HELP__(){
    cout << "NEDB version: 22.4.25 <Stable>" << endl;
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
