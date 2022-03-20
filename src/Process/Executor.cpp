#include<Process.h>
#include<unistd.h> 
#include <Data.h>
#include<Implement.h>

Executor::Executor(){
    __Statement = "";
    __Index_Front = -1;
    __Index_Tail = -1;
    __Table_Name = "";
    __Parameter = "";
}

void Executor::execute(__OPERATE__ operate,string statement){
    __Statement = Trim(statement);
    switch (operate){
        case __TABLE_CREATE :
            execute_create_table();
            break;
        case __INSERT :
            execute_insert_row();
            break;
        case __PRINT :
            execute_print_table();
            break;
        case __DELETE :
            execute_delete_row();
            break;
        case __TABLE_REMOVE :
            execute_remove_table();
            break;
        case __UNKNOWN :
            cout<<"Unknown Operation '"<<statement<<"'."<<endl;
            break;
        default:
            break;
    }
}

void Executor::execute(__COMMAND__ command,string statement){
    switch (command){
        case __UNK :
            printf("Unknown Command '%s'\n",Trim(statement).c_str());
            break;
        case __HELP :
            __HELP__();
            break;
        case __SAVE :
            printf("Saving to files...");
            sleep(2);
            printf("Done.\n");
            break;
        default:
            break;
    }
}

bool Executor::pre_process(string symbol){
    if(symbol.compare("{") == 0){
        __Index_Front = __Statement.find_first_of('{');
        __Index_Tail = __Statement.find_last_of('}');
    }else if(symbol.compare("(") == 0){
        __Index_Front = __Statement.find_first_of('(');
        __Index_Tail = __Statement.find_last_of(')');
    }
    if(__Index_Front < 0 || __Index_Tail < 0){
        cout<<"Unknown Statement."<<endl;
        return false;
    }
    __Table_Name = Trim(__Statement.substr(0,__Index_Front));
    __Parameter = Trim(__Statement.substr(__Index_Front+1, __Index_Tail - __Index_Front - 1));

    return true;
}


void Executor::execute_create_table(){
    if(!pre_process("(")) return;
    for(int i = 0;i<__CURSOR__;i++){
        if(__TABLES__[i]->getName() == __Table_Name){
            cout<<"Table '"<<__Table_Name<<"' Already Exists."<<endl;
            cout<<"Create Table '"<<__Table_Name<<"' Failed."<<endl;
            return;
        }
    }
    Table* new_table = new Table(__CURSOR__, __Table_Name);
    if(new_table->init(__Parameter)){
        __TABLES__[__CURSOR__] = new_table;
        ++ __CURSOR__ ;
        cout<<"Create Table '"<<__Table_Name<<"' Success."<<endl;
    }else{
        cout<<"Create Table '"<<__Table_Name<<"' Failed."<<endl;
    }
}

void Executor::execute_insert_row(){
    if(!pre_process("(")) return;
    int index = __TABLE_LOCATED_BY_NAME__(__Table_Name);
    if(index!=-1){
        if( ! __TABLES__[index]->insert(__Parameter)){
            cout<<"Insert To '"<<__Table_Name<<"' Failed."<<endl;
            return;
        }
        cout<<"Insert To '"<<__Table_Name<<"' Success."<<endl;
        return ;
    }
    cout<<"No Such Table Named '"<<__Table_Name<<"'."<<endl;
}

/*
    DELETE table_name(row_index);

*/


void Executor::execute_delete_row(){
    if(!pre_process("(")) return;
    int index = __TABLE_LOCATED_BY_NAME__(__Table_Name);
    if(index!=-1){
        string row_index = __Parameter;
        if(! __TABLES__[index]->delete_data(row_index)){
            cout<<"Delete Page From '"<<__Table_Name<<"' Failed."<<endl;
            return;
        }
        cout<<"Delete Page From '"<<__Table_Name<<"' Success."<<endl;
        return;
    }
    cout<<"No Such Table Named '"<<__Table_Name<<"'."<<endl;
}


void Executor::execute_remove_table(){
    int index = __TABLE_LOCATED_BY_NAME__(__Statement);
    if(index !=-1){
        __TABLES__[index]->remove_table();
        for(int i = index;i<__CURSOR__ -1;i++){
            __TABLES__ [i] = __TABLES__ [i+1];
        }
        -- __CURSOR__;
        cout<<"Remove Table '"<<__Statement<<"' Success."<<endl;
        return;
    }
    cout<<"No Such Table Named '"<<__Statement<<"'."<<endl;
}


void Executor::execute_print_table(){
    int index = __TABLE_LOCATED_BY_NAME__(__Statement);
    if(index!=-1){
        __TABLES__[index]->print_table();
        return ;
    }
    cout<<"No Such Table Named '"<<__Statement<<"'."<<endl;
}