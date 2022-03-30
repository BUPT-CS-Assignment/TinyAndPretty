#include <data.h>
#include <implement.h>

/////////////////////////////////////////////////////////////////////

void Table::InsertValues(string conditions, string values){
    /*
        在数据表中插入数据行
    */
    //填充数据
    Memorizer RAM;
    //查找插入数据页指针
    try{
        Row *new_row = new Row(this); //创建行对象
        new_row->Padding(conditions, values);
        __uint16_t *page_offset = pages_tree_->LocateData(&(new_row->getIndex())).getData();
        if(page_offset == NULL){
            /////计算偏移量
            __uint16_t *new_page_offset = new __uint16_t(get_empty_page_offset());
            /////创建新页
            Page *new_page = new Page(this);
            RAM.PageFlush(*new_page_offset, this);
            new_page->InsertRow(new_row);
            new_page->page_index_ = *new Index(new_row->getIndex());
            pages_tree_->InsertData(&(new_row->getIndex()), new_page_offset);
            RAM.PageStore(*new_page_offset, new_page);
        }
        /////读入内存//////////////////////////////////////////////////////
        Page *page = RAM.PageLoad(*page_offset, this);
        if(!page->is_full_) page->InsertRow(new_row);
        else{
            //////新建页后插入/////////////////////////////////////////////////
            Page *new_page = new Page(this);
            __uint16_t *new_page_offset = new __uint16_t(get_empty_page_offset());
            RAM.PageFlush(*new_page_offset, this);
            for(int i = max_rows_per_page_ / 2; i < max_rows_per_page_; i++){
                new_page->InsertRow(page->rows_[i]);
                page->cursor_pos_--;
            }
            page->is_full_ = false;
            new_page->InsertRow(new_row);
            new_page->page_index_ = *new Index(new_page->rows_[0]->getIndex());
            pages_tree_->InsertData(&(new_page->page_index_), new_page_offset);
            RAM.PageFlush(*page_offset, this);
            RAM.PageStore(*new_page_offset, new_page);
        }
        RAM.PageStore(*page_offset, page);
    }
    catch(NEexception &e){
        throw e;
    }
}

//插入行
void Page::InsertRow(Row *Node){
    try{
        for(__uint16_t i = 0; i < cursor_pos_; i++){
            if(rows_[i]->getIndex() == Node->getIndex()){
                throw KEY_VAL_EXIST;
            }
        }
        rows_[cursor_pos_++] = Node;
        q_sort(rows_, 0, cursor_pos_ - 1);
        if(cursor_pos_ >= table_ptr_->max_rows_per_page_){
            is_full_ = true;
        }
    }
    catch(NEexception &e){
        throw e;
    }
}

void Row::Padding(string condition, string value){
    /*
        填充行元素
    */
    try{
        if(value.length() == 0) throw PARAM_EMPTY;
        int cond_num = -1, value_num = -1;
        string *values = Split(value, ',', value_num);
        string *conds = Split(condition, ',', cond_num);
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
    catch(NEexception &e){
        throw e;
    }

}

void Row::update_value(int pos, string value){
    DATA_TYPE type = table_ptr_->parm_types_[pos];
    try{
        parm_check(value, type);
        switch(type){
            case __INT:
                try{
                    *((int *)content_[pos]) = stoi(value);
                }
                catch(out_of_range &e){
                    throw TYPE_INT_OVERFLOW;
                }
                return;
            case __INT64:
                try{
                    *(int *)content_[pos] = stoll(value);
                }
                catch(out_of_range &e){
                    throw TYPE_INT64_OVERFLOW;
                }
                return;
            case __REAL:
                try{
                    *(double *)content_[pos] = stod(value);
                }
                catch(out_of_range &e){
                    throw TYPE_REAL_OVERFLOW;
                }
                return;
            case __TEXT:
                memset(content_[pos], '\0', TEXT_LENGTH);
                strcpy((char *)content_[pos], value.c_str());
                return;
            case __LONGTEXT:
                memset(content_[pos], '\0', LONGTEXT_LENGTH);
                strcpy((char *)content_[pos], value.c_str());
                return;
            default:
                throw TYPE_UNDEFINED;
        }
    }
    catch(NEexception &e){
        throw e;
    }
}
