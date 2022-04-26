#include <Basic/data.h>
#include <Utils/implement.h>
#include <Basic/process.h>
using namespace std;

/////////////////////////////////////////////////////////////////////

void Table::InsertValues(string conditions, string values){
    /*
        在数据表中插入数据行
    */
    try{
        /* Lock Check */
        if(!StatusCheck(table_status_, SIG_FREE, SIG_CHECK_TIMES)){
            throw ACTION_BUSY;
        }
        table_status_ = SIG_RUN;
        Memorizer RAM(this);
        Row* new_row = new Row(this); //创建行对象
        new_row->Padding(conditions, values);
        /////
        Index temp(new_row->getIndex());
        __uint16_t* page_offset = pages_tree_->LocateData(temp).getData();
        if(page_offset == NULL){
            /////计算偏移量
            __uint16_t new_page_offset = get_empty_page_offset();
            /////创建新页
            Page* new_page = new Page(this);
            RAM.PageFlush(new_page_offset);
            new_page->InsertRow(new_row);
            new_page->page_index_ = temp;
            pages_tree_->InsertData(temp, new_page_offset);
            RAM.PageStore(new_page_offset, new_page);

            /* Set Lock */
            new_page->Erase();
            table_status_ = SIG_FREE;
            return;
        }
        /////读入内存//////////////////////////////////////////////////////
        Page* page = RAM.PageLoad(*page_offset);
        if(!page->is_full_) page->InsertRow(new_row);
        else{
            //////新建页后插入/////////////////////////////////////////////////
            Page* new_page = new Page(this);
            __uint16_t new_page_offset = get_empty_page_offset();
            RAM.PageFlush(new_page_offset);
            int line = page->cursor_pos_;
            for(int i = line / 2; i < line; i++){
                new_page->InsertRow(page->rows_[i]);
                page->rows_[i] = NULL;
                page->cursor_pos_--;
            }
            page->is_full_ = false;
            new_page->InsertRow(new_row);
            new_page->page_index_ = new_page->rows_[0]->getIndex();
            pages_tree_->InsertData(new_page->page_index_, new_page_offset);
            RAM.PageFlush(*page_offset);
            RAM.PageStore(new_page_offset, new_page);
            new_page->Erase();
        }
        RAM.PageStore(*page_offset, page);
        /* Set Lock */
        page->Erase();
        table_status_ = SIG_FREE;
    }
    catch(NEexception& e){
        table_status_ = SIG_FREE;
        throw e;
    }
    catch(exception& e){
        table_status_ = SIG_FREE;
        throw SYSTEM_ERROR;
    }
}

//插入行
void Page::InsertRow(Row* Node){
    try{
        if(Node == NULL){
            throw SYSTEM_ERROR;
        }
        //cout<<"Row Check "<<Node<<endl;
        //cout<<"Index Check "<<&(Node->getIndex())<<endl;
        Index origin(Node->getIndex());
        for(__uint16_t i = 0; i < cursor_pos_; i++){
            if(rows_[i]->getIndex() == origin){
                throw KEY_VAL_EXIST;
            }
        }
        rows_[cursor_pos_++] = Node;
        q_sort(rows_, 0, cursor_pos_ - 1);
        if(cursor_pos_ >= table_ptr_->max_rows_per_page_){
            is_full_ = true;
        }
    }
    catch(NEexception& e){
        throw e;
    }
    catch(exception& e){
        throw SYSTEM_ERROR;
    }
}

void Row::Padding(string condition, string value){
    /*
        填充行元素
    */
    try{
        if(value.length() == 0) throw PARAM_EMPTY;
        int cond_num = -1, value_num = -1;
        string* values = Split(value, ',', value_num);
        string* conds = Split(condition, ',', cond_num);
        //直接填充
        if(cond_num == 0 && value_num == table_ptr_->parm_num_){
            for(int i = 0; i < value_num; i++){
                update_value(i, values[i]);
            }
            index_update();
            return;
        }
        //判断元素个数匹配
        if(cond_num != value_num || cond_num > table_ptr_->parm_num_ ||
            value_num > table_ptr_->parm_num_){
            throw PARAM_NUM_MISMATCH;
        }
        //主键检测;
        if(condition.find(table_ptr_->parm_names_[table_ptr_->prim_key_]) ==
            condition.npos){
            throw KEY_VAL_REQUIRED;
        }
        //定位填充
        for(int i = 0; i < value_num; i++){
            int pos = table_ptr_->ParmLocate(conds[i]);
            if(pos == -1){
                throw PARAM_NOT_FOUND;
            }
            update_value(pos, values[i]);
        }
        index_update();
    }
    catch(NEexception& e){
        throw e;
    }
    catch(exception& e){
        throw SYSTEM_ERROR;
    }

}

void Row::update_value(int pos, string value){
    DATA_TYPE type = table_ptr_->parm_types_[pos];
    try{
        parm_check(value, type);
        switch(type){
            case __INT:
                try{
                    *((int*)content_[pos]) = stoi(value);
                }
                catch(out_of_range& e){
                    throw TYPE_INT_OVERFLOW;
                }
                return;
            case __INT64:
                try{
                    *(int*)content_[pos] = stoll(value);
                }
                catch(out_of_range& e){
                    throw TYPE_INT64_OVERFLOW;
                }
                return;
            case __REAL:
                try{
                    *(double*)content_[pos] = stod(value);
                }
                catch(out_of_range& e){
                    throw TYPE_REAL_OVERFLOW;
                }
                return;
            case __TEXT:
                memset(content_[pos], '\0', TEXT_LENGTH);
                strcpy((char*)content_[pos], value.c_str());
                return;
            case __LONGTEXT:
                memset(content_[pos], '\0', LONGTEXT_LENGTH);
                strcpy((char*)content_[pos], value.c_str());
                return;
            default:
                throw TYPE_UNDEFINED;
        }
    }
    catch(NEexception& e){
        throw e;
    }
    catch(exception& e){
        throw SYSTEM_ERROR;
    }
}
