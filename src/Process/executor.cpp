#include<Basic/process.h>
#include<Basic/data.h>
#include<Basic/storage.h>
using namespace std;
using namespace NEDBnamespace;

Executor::Executor(Parser *parser, DataBase *db){
    db_= db;
    parser_ = parser;
    returnValue_ = "";
    count_ = 0;
    errCode_ =0;

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
                returnValue_ = "HELP_INFO";
                __HELP__();
                break;
            case __MOUNTALL:
                db_->openall(count_);
                break;
            case __MOUNT:
                db_->open(parser_->statement_,RELATIVE_PATH);
                break;
            case __UNMOUNT:
                db_->dropTable(parser_->statement_,FILE_UNMOUNT);
                break;
            case __OPEN:
                db_->open(parser_->statement_,FULL_PATH);
                break;
            case __DEBUGSET:
                if(!__DEBUG_SET__(parser_->statement_)){
                    throw COMMAND_UNDEFINED;
                }
                break;
            case __SETDIR:
                if(parser_->statement_ == "-d"){
                    db_->setDir(DEFAULT_DIR);
                }
                else{
                    if(db_->setDir(parser_->statement_)!=NO_ERROR){
                        throw DIR_ERROR;
                    }
                }
                break;
            case __SHOWDIR:
                returnValue_ = db_->getDir();
                CONSOLE_LOG(0,TIME_FLAG,1,"Current Dir '%s'\n",returnValue_.c_str());
                break;
            case __DIRINIT:
                db_->dirInit();
                break;
            case __SHOWPAGESIZE:
                returnValue_ = to_string(db_->getDefaultPageSize());
                CONSOLE_LOG(0,TIME_FLAG,1,"PageSize '%d' Bytes\n",db_->getDefaultPageSize());
                break;
            case __SETPAGESIZE:
                if(parser_->statement_ == "-d"){
                    db_->setDefaultPageSize(DEFAULT_PAGE_SIZE);
                }
                else{
                    parm_check(parser_->statement_, __INT);
                    if(db_->setDefaultPageSize(stoi(parser_->statement_)) != NO_ERROR){
                        throw SIZE_NOT_ALLOWED;
                    }
                }
                break;
            default:
                throw COMMAND_UNDEFINED;
        }
        errCode_ = NO_ERROR;
    }
    catch(NEexception &e){
        errCode_ = e;
        throw e;
    }
    catch(exception &e){
        errCode_ = SYSTEM_ERROR;
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
        new_table->filePath_ = db_->getDir()+table_name;
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
            table->DeleteValues(conditions,count_);
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
        db_->dropTable(table_name,FILE_DROP);
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
            returnValue_ = table->getStructure();
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
            returnValue_ = table->SelectValues(parser_->value_, parser_->condition_,count_);
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
            table->UpdateValues(condition, values,count_);
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
        returnValue_ = str;
        count_ = db_->getCursor();
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

