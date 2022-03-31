#include<process.h>
#include<data.h>
#include<implement.h>
#include<storage.h>


Executor::Executor(Parser *parser){
    parser_ = parser;
}

void Executor::execute_operation(Table** table,int& c,string& str){
    try{
        switch(parser_->operation_){
            case CREATE_TABLE:
                return execute_create_table(table,c);
            case INSERT_VALUES:
                return execute_insert_values(table,c);
            case DELETE_VALUES:
                return execute_delete_values(table,c);
            case SELECT_VALUES:
                return execute_select_values(table,c,str);
            case UPDATE_VALUES:
                return execute_update_values(table,c);
            case DESCRIBE_TABLE:
                return execute_describe_table(table,c,str);
            case CREATE_INDEX:
                return execute_create_index(table,c);
            case DROP_INDEX:
                return execute_drop_index(table,c);
            case DROP_TABLE:
                return execute_drop_table(table,c);
            case UNDEFINED:
                str = parser_->statement_;
                break;
            default:    break;
        }
        throw SQL_UNDEFINED;
    }catch(NEexception &e){
        throw e;
    }
}

void Executor::execute_command(){
    try{
        switch(parser_->command_){
            case __UNKNOWN:
                cout << "UNDEFINED COMMAND '" << parser_->statement_ << "'." << endl;
                break;
            case __HELP:
                __HELP__();
                break;
            case __LOADALL:
                __LOAD_ALL__(__TABLES__,__CURSOR__);
                break;
            case __LOAD:
                __LOAD_FILE__(__TABLES__,__CURSOR__,parser_->statement_);
                break;
            case __SAVE:
                printf("Saving to files...");
                sleep(2);
                printf("Done.\n");
                break;
            default:
                break;
        }
    }catch(NEexception &e){
        throw e;
    }
}


void Executor::execute_create_table(Table** table,int& cursor){
    try{
        string table_name = parser_->object_;
        string parameters = parser_->value_;
        int index = __TABLE_LOCATED_BY_NAME__(table,cursor,table_name);
        if(index != -1){
            throw TABLE_EXIST;
        }
        ///////////////////////////////////////////////////////////文件操作
        Table *new_table = new Table(cursor, table_name);
        new_table->Init(parameters);
        table[cursor] = new_table;
        ++ cursor;
    }catch(NEexception &e){
        throw e;
    }
}


void Executor::execute_insert_values(Table** table,int cursor){
    try{
        string table_name = parser_->object_;
        string values = parser_->value_;
        string conditions = parser_->condition_;
        int index = __TABLE_LOCATED_BY_NAME__(table,cursor,table_name);
        if(index != -1){
            table[index]->InsertValues(conditions, values);
        }else{
            throw TABLE_NOT_FOUND;
        }
    }catch(NEexception &e){
        throw e;
    }
}

/*
    DELETE table_name(row_index);

*/

void Executor::execute_delete_values(Table** table,int cursor){
    try{
        string table_name = parser_->object_;
        string conditions = parser_->condition_;
        int index = __TABLE_LOCATED_BY_NAME__(table,cursor,table_name);
        if(index != -1){
            //if(conditions.length() == 0) return execute_drop_table();
            table[index]->DeleteValues(conditions);
        }else{
            throw TABLE_NOT_FOUND;
        }
    }catch(NEexception &e){
        throw e;
    }
}


void Executor::execute_drop_table(Table** table,int& cursor){
    try{
        string table_name = parser_->object_;
        int index = __TABLE_LOCATED_BY_NAME__(table,cursor,table_name);
        if(index != -1){
            table[index]->FreeTable();
            Memorizer RAM;
            RAM.TableDrop(table_name);
            for(int i = index; i < cursor - 1; i++){
                table[i] = table[i + 1];
            }
            -- cursor;
        }else{
            throw TABLE_EXIST;
        }
    }catch(NEexception &e){
        throw e;
    }
}

void Executor::execute_describe_table(Table** table,int cursor,string& str){
    try{
        int index = __TABLE_LOCATED_BY_NAME__(table,cursor,parser_->object_);
        if(index != -1){
            str = table[index]->getStructure();
        }else{
            throw TABLE_NOT_FOUND;
        }
    }catch(NEexception &e){
        throw e;
    }
}

void Executor::execute_create_index(Table** table,int cursor){
    try{
        int index = __TABLE_LOCATED_BY_NAME__(table,cursor,parser_->object_);
        if(index != -1){
            //table[index]->print_structure();
            return;
        }else{
            throw TABLE_NOT_FOUND;
        }
    }catch(NEexception &e){
        throw e;
    }
}

void Executor::execute_drop_index(Table** table,int cursor){
    try{
        int index = __TABLE_LOCATED_BY_NAME__(table,cursor,parser_->object_);
        if(index != -1){
            //table[index]->print_structure();
        }else{
            throw TABLE_NOT_FOUND;
        }
    }catch(NEexception &e){
        throw e;
    }
}

void Executor::execute_select_values(Table** table,int cursor,string& str){
    try{
        int index = __TABLE_LOCATED_BY_NAME__(table,cursor,parser_->object_);
        if(index != -1){
            str = table[index]->SelectValues(parser_->value_, parser_->condition_);
        }else{
            throw TABLE_NOT_FOUND;
        }
    }catch(NEexception &e){
        throw e;
    }
}

void Executor::execute_update_values(Table** table,int cursor){
    try{
        string table_name = parser_->object_;
        string values = parser_->value_;
        string condition = parser_->condition_;
        int index = __TABLE_LOCATED_BY_NAME__(table,cursor,table_name);
        if(index != -1){
            table[index]->UpdateValues(condition, values);
        }else{
            throw TABLE_NOT_FOUND;
        }
    }catch(NEexception &e){
        throw e;
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

