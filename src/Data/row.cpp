#include <data.h>
#include <implement.h>

Row::Row(Table *table){
    table_ptr_ = table;
    row_index_ = *new Index(table->getKeyType());
    content_ = new void *[table->parm_num_];
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
                content_[i] = new char[TEXT_LENGTH]{0};
                strcpy((char *)content_[i], "NULL");
                break;
            case __LONGTEXT:
                content_[i] = new char[LONGTEXT_LENGTH]{0};
                strcpy((char *)content_[i], "NULL");
                break;
        }
    }
}

/////////待优化///////////////////////////////////////////////////////





//格式化字符串
//Format  @  'element_1,_element_2,element_3,...'
string Row::Format(){

    string temp = *new string("{");
    for(int i = 0; i < table_ptr_->parm_num_; i++){
        switch(table_ptr_->parm_types_[i]){
            case __INT:
                temp = temp + to_string(*((int *)content_[i]));
                break;
            case __INT64:
                temp = temp + to_string(*((long long *)content_[i]));
                break;
            case __REAL:
                temp = temp + to_string(*((double *)content_[i]));
                break;
            default:
                string str = (char *)content_[i];
                temp = temp + str;
                break;
        }
        temp = temp + (i == table_ptr_->parm_num_ - 1 ? "}" : ",");
    }
    return temp;
}



void Row::index_update(){
    DATA_TYPE type = table_ptr_->parm_types_[table_ptr_->prim_key_];
    row_index_ = *new Index(type);
    switch(type){
        case __INT:
            row_index_.index_.i_index = *((int *)content_[table_ptr_->prim_key_]); break;
        case __INT64:
            row_index_.index_.l_index = *((long long *)content_[table_ptr_->prim_key_]); break;
        case __REAL:
            row_index_.index_.d_index = *((double *)content_[table_ptr_->prim_key_]); break;
        case __TEXT:
            strcpy(row_index_.index_.t_index, (char *)content_[table_ptr_->prim_key_]); break;
        default:
            break;
    }

}

Index &Row::getIndex(){
    return row_index_;
}

bool Row::operator<(Row &row){
    return row_index_ < row.row_index_;
}
bool Row::operator>(Row &row){
    return row_index_ > row.row_index_;
}
bool Row::operator==(Row &row){
    return row_index_ == row.row_index_;
}
bool Row::operator<=(Row &row){
    return row_index_ <= row.row_index_;
}
bool Row::operator>=(Row &row){
    return row_index_ >= row.row_index_;
}







