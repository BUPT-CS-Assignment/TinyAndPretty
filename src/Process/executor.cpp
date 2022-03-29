#include<Process.h>
#include<Data.h>
#include<Implement.h>
#include<Storage.h>


Executor::Executor(Parser* parser){
    parser_ = parser;
}

void Executor::execute_operation(){
    switch (parser_->operation_){
        case CREATE_TABLE :
            execute_create_table();
            break;
        case INSERT_VALUES :
            execute_insert_values();
            break;
        case DELETE_VALUES :
            execute_delete_values();
            break;
        case SELECT_VALUES :
            execute_select_values();
            break;
        case UPDATE_VALUES :
            execute_update_values();
            break;
        case DESCRIBE_TABLE :
            execute_describe_table();
            break;
        case CREATE_INDEX : 
            execute_create_index();
            break;
        case DROP_INDEX :
            execute_drop_index();
            break;
        case DROP_TABLE :
            execute_drop_table();
            break;
        case UNDEFINED :
            cout<<"UNDEFINED OPERATION '"<<parser_->statement_<<"'."<<endl;
            break;
        default:
            break;
    }
}

void Executor::execute_command(){
    switch (parser_->command_){
        case __UNKNOWN :
            cout<<"UNDEFINED COMMAND '"<<parser_->statement_<<"'."<<endl;
            break;
        case __HELP :
            __HELP__();
            break;
        case __LOAD :
            __LOAD_FILE__(parser_->statement_);
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


void Executor::execute_create_table(){
    string table_name = parser_->object_;
    string parameters = parser_->value_;
    for(int i = 0;i<__CURSOR__;i++){
        if(__TABLES__[i]->getName() == table_name){
            cout<<"Table '"<<table_name<<"' Already Exists."<<endl;
            cout<<"Create Table '"<<table_name<<"' Failed."<<endl;
            return;
        }
    }
    ///////////////////////////////////////////////////////////文件操作
    Table* new_table = new Table(__CURSOR__, table_name);
    if(new_table->Init(parameters)){
        __TABLES__[__CURSOR__] = new_table;
        ++ __CURSOR__ ;
        cout<<"Create Table '"<<table_name<<"' Success."<<endl;
    }else{
        cout<<"Create Table '"<<table_name<<"' Failed."<<endl;
    }
}


void Executor::execute_insert_values(){
    string table_name = parser_->object_;
    string values = parser_->value_;
    string conditions = parser_->condition_;
    int index = __TABLE_LOCATED_BY_NAME__(table_name);
    if(index!=-1){
        if(! __TABLES__[index]->InsertValues(conditions,values)){
            cout<<"Insert To '"<<table_name<<"' Failed."<<endl;
            return;
        }
        cout<<"Insert To '"<<table_name<<"' Success."<<endl;
    }else
        cout<<"No Such Table Named '"<<table_name<<"'."<<endl;
}

/*
    DELETE table_name(row_index);

*/

void Executor::execute_delete_values(){
    string table_name = parser_->object_;
    string conditions = parser_->condition_;
    int index = __TABLE_LOCATED_BY_NAME__(table_name);
    if(index!=-1){
        //if(conditions.length() == 0) return execute_drop_table();
        if(!__TABLES__[index]->DeleteValues(conditions)){
            cout<<"Delete From '"<<table_name<<"' Failed."<<endl;
            return;
        }
        cout<<"Delete From '"<<table_name<<"' Success."<<endl;
        return;
    }
    cout<<"No Such Table Named '"<<table_name<<"'."<<endl;
}


void Executor::execute_drop_table(){
    string table_name = parser_->object_;
    int index = __TABLE_LOCATED_BY_NAME__(table_name);
    if(index !=-1){
        //__TABLES__[index]->remove_table();
        for(int i = index;i<__CURSOR__ -1;i++){
            __TABLES__ [i] = __TABLES__ [i+1];
        }
        -- __CURSOR__;
        cout<<"Remove Table '"<<table_name<<"' Success."<<endl;
    }else
        cout<<"No Such Table Named '"<<table_name<<"'."<<endl;
}


void Executor::execute_print_table(){
    int index = __TABLE_LOCATED_BY_NAME__(parser_->object_);
    if(index!=-1){
        __TABLES__[index]->print_table();
        return ;
    }
    cout<<"No Such Table Named '"<<parser_->object_<<"'."<<endl;
}

void Executor::execute_describe_table(){
    int index = __TABLE_LOCATED_BY_NAME__(parser_->object_);
    if(index!=-1){
        __TABLES__[index]->print_structure();
        return ;
    }
    cout<<"No Such Table Named '"<<parser_->object_<<"'."<<endl;
}

void Executor::execute_create_index(){
    int index = __TABLE_LOCATED_BY_NAME__(parser_->object_);
    if(index!=-1){
        //__TABLES__[index]->print_structure();
        return ;
    }
    cout<<"No Such Table Named '"<<parser_->object_<<"'."<<endl;
}

void Executor::execute_drop_index(){
    int index = __TABLE_LOCATED_BY_NAME__(parser_->object_);
    if(index!=-1){
        //__TABLES__[index]->print_structure();
        return ;
    }
    cout<<"No Such Table Named '"<<parser_->object_<<"'."<<endl;
}

void Executor::execute_select_values(){
    int index = __TABLE_LOCATED_BY_NAME__(parser_->object_);
    if(index!=-1){
        cout<<__TABLES__[index]->SelectValues(parser_->condition_,parser_->value_)<<endl;
        return ;
    }
    cout<<"No Such Table Named '"<<parser_->object_<<"'."<<endl;
}

void Executor::execute_update_values(){
    cout<<"update test"<<endl;
}

/*
void Executor::execute_show_tables(){
    cout<<"+---------------"<<endl;
    cout<<"|    TABLES     "<<endl;
    cout<<"+---------------"<<endl;
    for(int i = 0; i<__CURSOR__; i++){
        cout<<"| "<<__TABLES__[i]->getName()<<endl;
    }
    cout<<"+---------------"<<endl;
}*/


