#include<Basic/process.h>
#include<Basic/data.h>
#include<Utils/implement.h>
#include<Basic/storage.h>
using namespace std;

Executor::Executor(Parser *parser, DataBase *db){
    db_ = db;
    parser_ = parser;
}

void Executor::execute_operation(){
    try{
        if(parser_->operation_ == DROP_TABLE){
            execute_drop_table();
            return;
        }
        if(parser_->operation_ == CREATE_TABLE){
            execute_create_table();
            return;
        }
        if(!StatusCheck(db_->Status(),SIG_RUN,SIG_CHECK_TIMES)){
            throw ACTION_BUSY;
        }
        db_->SetStatus(SIG_RUN);
        switch(parser_->operation_){
            case INSERT_VALUES:
                execute_insert_values();
                break;
            case DELETE_VALUES:
                execute_delete_values();
                break;
            case SELECT_VALUES:
                execute_select_values();
                break;
            case UPDATE_VALUES:
                execute_update_values();
                break;
            case DESCRIBE_TABLE:
                execute_describe_table();
                break;
            case SELECT_TABLES:
                execute_select_tables();
                break;
            default:
                db_->SetStatus(SIG_FREE);
                throw SQL_UNDEFINED;
        }
        db_->SetStatus(SIG_FREE);
    }
    catch(NEexception &e){
        db_->SetStatus(SIG_FREE);
        throw e;
    }
    catch(exception &e){
        db_->SetStatus(SIG_FREE);
        throw SYSTEM_ERROR;
    }
}

void Executor::execute_command(){
    try{
        switch(parser_->command_){
            case __HELP:
                __HELP__();
                break;
            case __LOADALL:
                db_->openall();
                break;
            case __LOAD:
                db_->open(parser_->statement_);
                break;
            case __DEBUGSET:
                if(!__DEBUG_SET__(parser_->statement_)){
                    throw COMMAND_UNDEFINED;
                }
                break;
            case __SETDIR:
                if(parser_->statement_ == "-d"){
                    db_->setDir(__DefaultDir__);
                }
                else{
                    if(!db_->setDir(parser_->statement_)){
                        throw DIR_ERROR;
                    }
                }
                break;
            case __SHOWDIR:
                cout << db_->getDir() << endl;
                break;
            case __DIRINIT:
                db_->dirInit();
                break;
            case __SHOWPAGESIZE:
                cout << db_->getDefaultPageSize() << endl;
                break;
            case __SETPAGESIZE:
                if(parser_->statement_ == "-d"){
                    db_->setDefaultPageSize(DEFAULT_PAGE_SIZE);
                }
                else{
                    parm_check(parser_->statement_, __INT);
                    if(!db_->setDefaultPageSize(stoi(parser_->statement_))){
                        throw SIZE_NOT_ALLOWED;
                    }
                }
                break;
            default:
                throw COMMAND_UNDEFINED;
        }
        db_->setErrCode(NO_ERROR);
    }
    catch(NEexception &e){
        db_->setErrCode(e);
        throw e;
    }
    catch(exception &e){
        db_->setErrCode(SYSTEM_ERROR);
        throw SYSTEM_ERROR;
    }
}


void Executor::execute_create_table(){
    try{
        if(db_->getCursor() >= MAX_TABLES){
            throw TABLE_NUM_REACH_LIMIT;
        }
        string table_name = parser_->object_;
        string parameters = parser_->value_;
        if(!StatusCheck(db_->Status(),SIG_FREE,SIG_CHECK_TIMES)){
            throw ACTION_BUSY;
        }
        db_->SetStatus(SIG_BLOCK);
        Table *table = db_->getTable(parser_->object_);
        if(table != NULL){
            throw TABLE_EXIST;
        }
        ///////////////////////////////////////////////////////////文件操作
        Table *new_table = new Table(db_, table_name);
        new_table->Init(parameters);
        /* DataBase Lock Check */
        db_->addTable(new_table);
        db_->SetStatus(SIG_FREE);
    }
    catch(NEexception &e){
        db_->SetStatus(SIG_FREE);
        throw e;
    }
    catch(exception &e){
        db_->SetStatus(SIG_FREE);
        throw SYSTEM_ERROR;
    }
}


void Executor::execute_insert_values(){
    try{
        string table_name = parser_->object_;
        string values = parser_->value_;
        string conditions = parser_->condition_;
        Table *table = db_->getTable(parser_->object_);
        if(table != NULL){
            table->InsertValues(conditions, values);
        }
        else{
            throw TABLE_NOT_FOUND;
        }
    }
    catch(NEexception &e){
        throw e;
    }
    catch(exception &e){
        throw SYSTEM_ERROR;
    }
}

/*
    DELETE table_name(row_index);

*/

void Executor::execute_delete_values(){
    try{
        string table_name = parser_->object_;
        string conditions = parser_->condition_;
        Table *table = db_->getTable(parser_->object_);
        if(table != NULL){
            table->DeleteValues(conditions);
        }
        else{
            throw TABLE_NOT_FOUND;
        }
    }
    catch(NEexception &e){
        throw e;
    }
    catch(exception &e){
        throw SYSTEM_ERROR;
    }
}


void Executor::execute_drop_table(){
    try{
        string table_name = parser_->object_;
        db_->dropTable(table_name);
    }
    catch(NEexception &e){
        throw e;
    }
    catch(exception &e){
        throw SYSTEM_ERROR;
    }
}

void Executor::execute_describe_table(){
    try{
        Table *table = db_->getTable(parser_->object_);
        if(table != NULL){
            string str = table->getStructure();
            db_->setReturnVal(str);
        }
        else{
            throw TABLE_NOT_FOUND;
        }
    }
    catch(NEexception &e){
        throw e;
    }
    catch(exception &e){
        throw SYSTEM_ERROR;
    }
}


void Executor::execute_select_values(){
    try{
        Table *table = db_->getTable(parser_->object_);
        if(table != NULL){
            string str = table->SelectValues(parser_->value_, parser_->condition_);
            db_->setReturnVal(str);
        }
        else{
            throw TABLE_NOT_FOUND;
        }
    }
    catch(NEexception &e){
        throw e;
    }
    catch(exception &e){
        throw SYSTEM_ERROR;
    }
}

void Executor::execute_update_values(){
    try{
        string table_name = parser_->object_;
        string values = parser_->value_;
        string condition = parser_->condition_;
        Table *table = db_->getTable(parser_->object_);
        if(table != NULL){
            table->UpdateValues(condition, values);
        }
        else{
            throw TABLE_NOT_FOUND;
        }
    }
    catch(NEexception &e){
        throw e;
    }
    catch(exception &e){
        throw SYSTEM_ERROR;
    }
}

void Executor::execute_select_tables(){
    try{
        string str = "";
        int num = db_->getCursor();
        for(int i = 0; i < num; i++){
            Table* table_ptr = db_->getTable(i);
            if(table_ptr == NULL) continue;
            str = str + table_ptr->getName() + (i == num- 1 ? "" : ",");
        }
        db_->setReturnVal(str);
        db_->setCount(db_->getCursor());
    }
    catch(NEexception &e){
        throw e;
    }
    catch(exception &e){
        throw SYSTEM_ERROR;
    }
}

/*
void Executor::execute_show_tables(){
    cout<<"+---------------"<<endl;
    cout<<"|    TABLES     "<<endl;
    cout<<"+---------------"<<endl;
    for(int i = 0; i<cursor; i++){
        cout<<"| "<<table[i]->getName()<<endl;
    }
    cout<<"+---------------"<<endl;
}*/

/*
void Executor::execute_print_table(){
    int index = __TABLE_LOCATED_BY_NAME__(parser_->object_);
    if(index != -1){
        table[index]->print_table();
        return;
    }
    cout << "No Such Table Named '" << parser_->object_ << "'." << endl;
}
*/

