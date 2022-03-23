#include<Process.h>
 
#include<Data.h>
#include<Implement.h>
#include<Storage.h>

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
        case __SHOW :
            execute_show_tables();
            break;
        case __CREATE :
            execute_create_table();
            break;
        case __INSERT :
            execute_insert_row();
            break;
        case __CHECK :
            execute_print_table();
            break;
        case __DELETE :
            execute_delete_row();
            break;
        case __REMOVE :
            execute_remove_table();
            break;
        case __DESCRIBE :
            execute_describe_table();
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
        case __LOAD :
            __LOAD_FILE__(statement);
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
        
        return false;
    }
    __Table_Name = Trim(__Statement.substr(0,__Index_Front));
    __Parameter = Trim(__Statement.substr(__Index_Front+1, __Index_Tail - __Index_Front - 1));

    return true;
}


void Executor::execute_create_table(){
    if(!pre_process("(")){
        cout<<"Unknown Statement."<<endl;
        return;
    }
    for(int i = 0;i<__CURSOR__;i++){
        if(__TABLES__[i]->getName() == __Table_Name){
            cout<<"Table '"<<__Table_Name<<"' Already Exists."<<endl;
            cout<<"Create Table '"<<__Table_Name<<"' Failed."<<endl;
            return;
        }
    }
    ///////////////////////////////////////////////////////////文件操作
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
    if(!pre_process("(")){
        cout<<"Unknown Statement."<<endl;
        return;
    }
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
    if(!pre_process("(")){
        cout<<"Unknown Statement."<<endl;
        return;
    }
    int index = __TABLE_LOCATED_BY_NAME__(__Table_Name);
    if(index!=-1){
        Index row_index(__Parameter, __TABLES__[index]->getKeyType());
        if(! __TABLES__[index]->delete_row(row_index)){
            cout<<"Delete From '"<<__Table_Name<<"' Failed."<<endl;
            return;
        }
        cout<<"Delete From '"<<__Table_Name<<"' Success."<<endl;
        return;
    }
    cout<<"No Such Table Named '"<<__Table_Name<<"'."<<endl;
}


void Executor::execute_remove_table(){
    if(pre_process("(")){
        int index = __TABLE_LOCATED_BY_NAME__(__Table_Name);
        if(index == -1){
            cout<<"No Such Table Named '"<<__Statement<<"'."<<endl;
            return;
        }
        Index page_index(__Parameter, __TABLES__[index]->getKeyType());
        if(! __TABLES__[index]->delete_page(page_index)){
            cout<<"Delete From '"<<__Table_Name<<"' Failed."<<endl;
            return;
        }
        cout<<"Delete From '"<<__Table_Name<<"' Success."<<endl;
        return;
    }
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

void Executor::execute_describe_table(){
    int index = __TABLE_LOCATED_BY_NAME__(__Statement);
    if(index!=-1){
        __TABLES__[index]->print_structure();
        return ;
    }
    cout<<"No Such Table Named '"<<__Statement<<"'."<<endl;
}


void Executor::execute_show_tables(){
    cout<<"+---------------"<<endl;
    cout<<"|    TABLES     "<<endl;
    cout<<"+---------------"<<endl;
    for(int i = 0; i<__CURSOR__; i++){
        cout<<"| "<<__TABLES__[i]->getName()<<endl;
    }
    cout<<"+---------------"<<endl;
}


