#include<Process.h>
#include<Implement.h>

Parser::Parser(){
    statement_ = "";
    object_ = "";
    condition_ = "";
    value_ = "";
    operation_ = UNDEFINED;
    command_ = __UNKNOWN;
}

void Parser::flush(){
    statement_ = "";
    object_ = "";
    condition_ = "";
    value_ = "";
    operation_ = UNDEFINED;
    command_ = __UNKNOWN;
}

void Parser::analyse(string input){
    if(input[0] != '.'){
        command_ = __OPERATION;
        operation_ = operate_type(input);
        statement_ = Trim(input);
        if(!deconstruct())  cout << "WRONG OPERATION FROMAT" << endl;
    }
    else if(input.compare(".exit") == 0){
        command_ = __EXIT;
    }
    else if(input.compare(".help") == 0){
        command_ = __HELP;
    }
    else if(input.compare(".save") == 0){
        command_ = __SAVE;
    }
    else if(input.compare(".openall") == 0){
        command_ = __LOADALL;
    }
    else if(input.substr(0, input.find(" ")).compare(".open") == 0){
        command_ = __LOAD;
        int index = input.find(" ");
        statement_ = input.substr(index + 1, input.length() - index - 1);
    }
    else{
        command_ = __UNKNOWN;
        statement_ = input;
    }

}

bool Parser::deconstruct(){
    switch(operation_){
        case CREATE_TABLE:
            if(!parser_create_table()) return false;
            break;
        case INSERT_VALUES:
            if(!parser_insert_values()) return false;
            break;
        case DELETE_VALUES:
            if(!parser_delete_values()) return false;
            break;
        case SELECT_VALUES:
            if(!parser_select_values()) return false;
            break;
        case UPDATE_VALUES:
            if(!parser_update_values()) return false;
            break;
        case DESCRIBE_TABLE: case DROP_TABLE:
            if(!parser_describe_table()) return false;
            break;
        default:
            break;
    }
    return true;
}

bool Parser::parser_create_table(){
    regex layout("create table (.+)\\((.+)\\)");
    smatch result;
    if(regex_match(statement_, result, layout)){
        auto it = result.begin();
        object_ = Trim(*++it);
        value_ = Trim(*++it);
    }
    else return false;
    return true;

    /*
    statement_ = Trim(statement_.substr(13,statement_.length()-13));
    int index = statement_.find("(");
    object_ = Trim(statement_.substr(0,index));
    return take_out(statement_,value_);
    */
}

bool Parser::parser_insert_values(){
    regex layout("insert into (.+) \\((.+)\\) values \\((.+)\\)");
    regex layout2("insert into (.+) values \\((.+)\\)");
    smatch result;
    if(regex_match(statement_, result, layout)){
        auto it = result.begin();
        object_ = Trim(*++it);
        condition_ = Trim(*++it);
        value_ = Trim(*++it);
    }
    else if(regex_match(statement_, result, layout2)){
        auto it = result.begin();
        object_ = Trim(*++it);
        value_ = Trim(*++it);
    }
    else return false;
    return true;
    /*
    statement_ = Trim(statement_.substr(12,statement_.length()-12));
    int index = statement_.find(" ");
    object_ = statement_.substr(0,index);
    index = statement_.find("VALUES");
    if(index == 1) return false;
    string str1 = statement_.substr(0,index);
    string str2 = statement_.substr(index,statement_.length()-index);
    if(!take_out(str1,condition_)) return false;
    return take_out(str2,value_);*/
}

bool Parser::parser_delete_values(){
    regex layout("delete from (.+) where (.+)");
    //regex layout2("DELETE FROM (.+)");
    smatch result;
    if(regex_match(statement_, result, layout)){
        auto it = result.begin();
        object_ = Trim(*++it);
        condition_ = Trim(*++it);
    }
    else{
        object_ = Trim(statement_.substr(12, statement_.length() - 12));
    }
    return true;
    /*
    statement_ = Trim(statement_.substr(12,statement_.length()-12));
    int index = statement_.find(" WHERE ");
    if(index == -1) object_ = statement_;
    else{
        object_ = statement_.substr(0,index);
        condition_ = statement_.substr(index+6,statement_.length()-index-6);
        return true;
    }
    return true;
    */
}

bool Parser::parser_select_values(){
    regex layout("select (.+) from (.+) where (.+)");
    regex layout2("select (.+) from (.+)");
    smatch result;
    if(regex_match(statement_, result, layout)){
        auto it = result.begin();
        value_ = Trim(*++it);
        object_ = Trim(*++it);
        condition_ = Trim(*++it);
    }
    else if(regex_match(statement_, result, layout2)){
        auto it = result.begin();
        value_ = Trim(*++it);
        object_ = Trim(*++it);
    }
    else return false;
    return true;
    /*
    statement_ = Trim(statement_.substr(7,statement_.length()-7));
    int index = statement_.find(" FROM ");
    if(index == -1) return false;
    value_ = statement_.substr(0,index);
    statement_ = statement_.substr(index+6,statement_.length()-index-6);
    index = statement_.find(" WHERE ");
    if(index == -1) object_ = statement_;
    */

}

bool Parser::parser_update_values(){
    regex layout("update (.+) set (.+) where (.+)");
    regex layout2("update (.+) set (.+)");
    smatch result;
    if(regex_match(statement_, result, layout)){
        auto it = result.begin();
        object_ = Trim(*++it);
        value_ = Trim(*++it);
        condition_ = Trim(*++it);
    }
    else if(regex_match(statement_, result, layout2)){
        auto it = result.begin();
        object_ = Trim(*++it);
        value_ = Trim(*++it);
    }
    else return false;
    return true;
}

bool Parser::parser_describe_table(){
    object_ = Trim(statement_.substr(15, statement_.length() - 15));
    if(object_.length() == 0) return false;
    return true;
}

bool Parser::parser_create_index(){
    regex layout("create index (.+) on (.+)");
    smatch result;
    if(regex_match(statement_, result, layout)){
        auto it = result.begin();
        condition_ = Trim(*++it);
        object_ = Trim(*++it);
    }
    else return false;
    return true;
}

bool Parser::parser_drop_index(){
    regex layout("drop index (.+) on (.+)");
    smatch result;
    if(regex_match(statement_, result, layout)){
        auto it = result.begin();
        condition_ = Trim(*++it);
        object_ = Trim(*++it);
    }
    else return false;
    return true;
}

OPERATION  Parser::operate_type(string input){
    /******************/
    //if(regex_match(input,regex("CREATE TABLE( +)\\w+"))) return CREATE_TABLE;
    if(input.compare(0, 13, "create table ") == 0) return CREATE_TABLE;
    //if(regex_match(input,regex("INSERT INTO( +)\\w+"))) return INSERT_VALUES;
    if(input.compare(0, 12, "insert into ") == 0) return INSERT_VALUES;
    //if(regex_match(input,regex("DELETE FROM( +)\\w+"))) return DELETE_VALUES;
    if(input.compare(0, 12, "delete from ") == 0) return DELETE_VALUES;
    //if(regex_match(input,regex("SELECT ( +)\\w+"))) return SELECT_VALUES;
    if(input.compare(0, 7, "select ") == 0) return SELECT_VALUES;
    //if(regex_match(input,regex("UPDATE( +)\\w+"))) return UPDATE_VALUES;
    if(input.compare(0, 7, "update ") == 0) return UPDATE_VALUES;
    //if(regex_match(input,regex("DROP TABLE( +)\\w+"))) return DROP_TABLE;
    if(input.compare(0, 11, "drop table ") == 0) return DROP_TABLE;
    //if(regex_match(input,regex("DESCRIBE TABLE( +)\\w+"))) return DESCRIBE_TABLE;
    if(input.compare(0, 15, "describe table ") == 0) return DESCRIBE_TABLE;
    //if(regex_match(input,regex("CREATE INDEX( +)\\w+"))) return CREATE_INDEX;
    if(input.compare(0, 13, "create index ") == 0) return CREATE_INDEX;
    //if(regex_match(input,regex("DROP INDEX( +)\\w+"))) return DROP_INDEX;
    if(input.compare(0, 11, "drop index ") == 0) return DROP_INDEX;
    return UNDEFINED;
}

string Parser::getStatment(){
    return statement_;
}

COMMAND Parser::getCommand(){
    return command_;
}

OPERATION Parser::getOperate(){
    return operation_;
}

