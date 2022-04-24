#include <Basic/data.h>
#include <Utils/implement.h>
using namespace std;

Row::Row(Table* table){
    table_ptr_ = table;
    row_lock_ = SIG_UNLOCK;
    row_index_.setVal(table->getKeyType(), NULL);
    content_ = new void* [table->parm_num_];
    for(int i = 0; i < table->parm_num_; i++){
        //初始化行数据
        DATA_TYPE type = table->parm_types_[i];
        switch(type){
            case __INT:
                content_[i] = new int(0);
                break;
            case __INT64:
                content_[i] = new long long(0LL);
                break;
            case __REAL:
                content_[i] = new double(0.0);
                break;
            case __TEXT:
                content_[i] = new char[TEXT_LENGTH];
                strcpy((char*)content_[i], "NULL");
                break;
            case __LONGTEXT:
                content_[i] = new char[LONGTEXT_LENGTH];
                strcpy((char*)content_[i], "NULL");
                break;
        }
    }
}

/////////待优化///////////////////////////////////////////////////////





//格式化字符串
//Format  @  'element_1,_element_2,element_3,...'
string Row::Format(){
    string temp = "{";
    for(int i = 0; i < table_ptr_->parm_num_; i++){
        switch(table_ptr_->parm_types_[i]){
            case __INT:
                temp = temp + to_string(*((int*)content_[i]));
                break;
            case __INT64:
                temp = temp + to_string(*((long long*)content_[i]));
                break;
            case __REAL:
                temp = temp + to_string(*((double*)content_[i]));
                break;
            default:
                string str = (char*)content_[i];
                temp = temp + str;
                break;
        }
        temp = temp + (i == table_ptr_->parm_num_ - 1 ? "}" : ",");
    }
    return temp;
}


void Row::Erase(){
    try{
        row_lock_ = SIG_LOCK;
        for(int i = 0; i < table_ptr_->parm_num_; i++){
            DATA_TYPE type = table_ptr_->parm_types_[i];
            switch(type){
                case __INT:
                    delete (int*)content_[i];
                    break;
                case __INT64:
                    delete (long long*)content_[i];
                    break;
                case __REAL:
                    delete (double*)content_[i];
                    break;
                case __TEXT: case __LONGTEXT:
                    delete (char*)content_[i];
                    break;
            }
            content_[i] = NULL;
        }
        content_ = NULL;
        table_ptr_ = NULL;

    }
    catch(NEexception& e){
        throw e;
    }
    catch(exception& e){
        throw SYSTEM_ERROR;
    }
}


void Row::index_update(){
    DATA_TYPE type = table_ptr_->parm_types_[table_ptr_->prim_key_];
    row_index_.setVal(type, content_[table_ptr_->prim_key_]);
}

Index& Row::getIndex(){
    return row_index_;
}

bool Row::operator<(Row& row){
    return row_index_ < row.row_index_;
}
bool Row::operator>(Row& row){
    return row_index_ > row.row_index_;
}
bool Row::operator==(Row& row){
    return row_index_ == row.row_index_;
}
bool Row::operator<=(Row& row){
    return row_index_ <= row.row_index_;
}
bool Row::operator>=(Row& row){
    return row_index_ >= row.row_index_;
}







