#include <Data.h>
#include <Implement.h>

/////////////////////////////////////////////////////////////////////

bool Table::InsertValues(string conditions, string values){
    /*
        在数据表中插入数据行
    */
    Row *new_row = new Row(this); //创建行对象
    //填充数据
    if(!new_row->Padding(conditions, values)){
        return false;
    }
    /////////////////////////////////////////SecondaryIndexTreeCheck///////////////////////
    /*
    for(int i = 0; i < parm_num_; i++){
        if(index_tree_[i]!=NULL) index_tree_[i]->CheckAll();
    }*/
    Memorizer T;
    //查找插入数据页指针
    __uint16_t *page_offset = pages_tree_->InsertLocate(&(new_row->getIndex()));
    if(page_offset == NULL){
        /////计算偏移量
        __uint16_t *new_page_offset = new __uint16_t(get_empty_page_offset());
        /////创建新页
        Page *new_page = new Page(this);
        T.PageFlush(*new_page_offset, this);
        new_page->InsertRow(new_row);
        new_page->page_index_ = *new Index(new_row->getIndex());
        pages_tree_->InsertData(&(new_row->getIndex()), new_page_offset);
        T.PageStore(*new_page_offset, new_page);
        //T.TableUpdate(this);
        return true;
    }
    /////读入内存//////////////////////////////////////////////////////
    Page *page = T.PageLoad(*page_offset, this);
    if(!page->is_full_){
        if(!page->InsertRow(new_row)){
            cout << "<E> PRIM KEY VALUE '" << new_row->getIndex() << "' EXISTS."
                << endl;
            return false;
        }
    }
    else{
        //////新建页后插入/////////////////////////////////////////////////
        Page *new_page = new Page(this);
        __uint16_t *new_page_offset = new __uint16_t(get_empty_page_offset());
        T.PageFlush(*new_page_offset, this);
        for(int i = max_rows_per_page_ / 2; i < max_rows_per_page_; i++){
            new_page->InsertRow(page->rows_[i]);
            // page->rows_[i]->Erase();
            page->cursor_pos_--;
        }
        page->is_full_ = false;
        new_page->InsertRow(new_row);
        new_page->page_index_ = *new Index(new_page->rows_[0]->getIndex());
        pages_tree_->InsertData(&(new_page->page_index_), new_page_offset);
        T.PageFlush(*page_offset, this);
        T.PageStore(*new_page_offset, new_page);
    }
    T.PageStore(*page_offset, page);
    //T.TableStore(this);
    return true;
}

//插入行
bool Page::InsertRow(Row *Node){
    for(__uint16_t i = 0; i < cursor_pos_; i++){
        if(rows_[i]->getIndex() == Node->getIndex())
            return false;
    }
    rows_[cursor_pos_++] = Node;
    q_sort(rows_, 0, cursor_pos_ - 1);
    cout << "State : " << cursor_pos_ << " / " << table_ptr_->max_rows_per_page_
        << endl;
    if(cursor_pos_ >= table_ptr_->max_rows_per_page_)
        is_full_ = true;
    return true;
}

bool Row::Padding(string condition, string value){
    /*
        填充行元素
        INSERT INTO table_name (ID,NAME,...) VALUES (1,name);
    */
    if(value.length() == 0){
        cout << "<E> VALUES EMPTY" << endl;
        return false;
    }
    int cond_num = -1, value_num = -1;
    string *values = Split(value, ',', value_num);
    string *conds = Split(condition, ',', cond_num);
    //直接填充
    if(cond_num == 0 && value_num == table_ptr_->parm_num_){
        for(int i = 0; i < value_num; i++){
            if(!value_update(i, values[i])){
                return false;
            }
        }
        index_update();
        return true;
    }
    //判断元素个数匹配
    if(cond_num != value_num || cond_num > table_ptr_->parm_num_ ||
        value_num > table_ptr_->parm_num_){
        cout << "<E> PARAMETER NUMBER MISMATCH" << endl;
        return false;
    }
    //主键检测;
    if(condition.find(table_ptr_->parm_names_[table_ptr_->prim_key_]) ==
        condition.npos){
        cout << "<E> PRIM KEY VALUE REQUIRED" << endl;
        return false;
    }
    //定位填充
    for(int i = 0; i < value_num; i++){
        int pos = table_ptr_->ParmLocate(conds[i]);
        if(pos == -1){
            cout << "<E> PARAMETER NOT FOUND" << endl;
            return false;
        }
        if(!value_update(pos, values[i]))
            return false;
    }
    index_update();
    return true;
}

bool Row::value_update(int pos, string value){
    DATA_TYPE type = table_ptr_->parm_types_[pos];
    if(!parm_check(value, type))
        return false;
    switch(type){
        case __INT:
            try{
                *((int *)content_[pos]) = stoi(value);
            }
            catch(out_of_range &e){
                cout << "<E> OUT OF RANGE 'INT'" << endl;
                return false;
            }
            return true;
        case __INT64:
            try{
                *(int *)content_[pos] = stoll(value);
            }
            catch(out_of_range &e){
                cout << "<E> OUT OF RANGE 'INT64'" << endl;
                return false;
            }
            return true;
        case __REAL:
            try{
                *(double *)content_[pos] = stod(value);
            }
            catch(out_of_range &e){
                cout << "<E> OUT OF RANGE 'REAL'" << endl;
                return false;
            }
            return true;
        case __TEXT:
            memset(content_[pos], '\0', TEXT_LENGTH);
            strcpy((char *)content_[pos], value.c_str());
            return true;
        case __LONGTEXT:
            memset(content_[pos], '\0', LONGTEXT_LENGTH);
            strcpy((char *)content_[pos], value.c_str());
            return true;
        default:
            return false;
    }
}
