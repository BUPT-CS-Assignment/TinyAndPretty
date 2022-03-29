#include<Data.h>
#include<Implement.h>

bool Table::DeleteValues(string conditions){
    if(conditions.length() == 0){
        return delete_by_traverse(-1, "");
    }
    int l = 0;
    //分离删除条件 暂时仅支持"="
    string *cond = Split(conditions, '=', l);
    if(l == 0) return false;
    //定位删除元素名称
    int index = ParmLocate(cond[0]);
    if(index == -1) return false;
    //确定是否为主键
    if(index == prim_key_){
        return delete_by_key(index, cond[1]);
    }
    else{
        return delete_by_traverse(index, cond[1]);
        //非主键元素, 查询相应非主键索引, 回表
        //if(index_tree_[index] == NULL){  
        //}
        //查询非主键索引, 获得主键索引链表
        //Index *sec_index = new Index(cond[1], parm_types_[index]);
        //return true;
    }
    return true;
}

bool Table::delete_by_key(int pos, string value){
    Index *temp = new Index(value, parm_types_[pos]);
    __uint16_t *page_offset = pages_tree_->InsertLocate(temp);
    if(page_offset == NULL){
        cout << "<E> DATA NOT FOUND" << endl;
        return false;
    }
    Memorizer RAM;
    Page *page = RAM.PageLoad(*page_offset, this);
    if(!page->DeleteRow(pos, value)){
        return false;
    }
    if(page->cursor_pos_ == 0){
        page->Clear(*page_offset);
    }
    else{
        RAM.PageStore(*page_offset, page);
    }
    //RAM.TableStore(this);
    return true;
}

bool Table::delete_by_traverse(int cond_pos, string cond_value){
    //遍历搜索
    Node<__uint16_t, Index> *head = pages_tree_->getHead();
    if(head == NULL) return "";
    Memorizer RAM;
    while(head != NULL){
        for(int i = 0; i >= 0; i++){
            __uint16_t *page_offset = head->getData(i);
            if(page_offset == NULL) break;
            Page *page = RAM.PageLoad(*page_offset, this);
            if(!page->DeleteRow(cond_pos, cond_value)){
                return false;
            }
            if(page->cursor_pos_ == 0){
                page->Clear(*page_offset);
                i --;
            }
            else{
                RAM.PageStore(*page_offset, page);
            }
        }
        head = head->getNext();
    }
    //RAM.TableStore(this);
    return true;
}

void Page::Clear(__uint16_t offset){
    if(offset != table_ptr_->max_offset - 1){
        table_ptr_->add_empty_page(offset);
    }
    else{
        table_ptr_->max_offset --;
    }
    table_ptr_->pages_tree_->DeleteData(&page_index_);
    Memorizer RAM;
    RAM.PageFlush(offset, table_ptr_);
}

bool Page::DeleteRow(int pos, string value){
    if(pos == -1){
        for(int i = 0; i < cursor_pos_; i++){
            rows_[i]->Erase();
        }
        cursor_pos_ = 0;
        return true;
    }
    if(pos == table_ptr_->prim_key_){
        int delete_num = 0;
        for(int i = 0; i < cursor_pos_; i++){
            if(rows_[i]->get_value(pos) == value){
                delete_num ++;
                rows_[i]->Erase();
                for(int j = i; j < cursor_pos_ - 1; j++){
                    rows_[j] = rows_[j + 1];
                }
                cursor_pos_--;
                break;
            }
        }
        if(delete_num == 0){
            cout<<"<E> DATA NOT FOUND"<<endl;
            return false;
        }   
    }
    else{
        for(int i = 0; i < cursor_pos_; i++){
            if(rows_[i]->get_value(pos) == value){
                rows_[i]->Erase();
                rows_[i] = NULL;
            }
        }
        for(int i = 0; i < cursor_pos_; i++){
            if(rows_[i] == NULL){
                for(int j = i; j < cursor_pos_ - 1; j++){
                    rows_[j] = rows_[j + 1];
                }
                cursor_pos_--;
                i--;
            }
        }
    }
    return true;
}

void Row::Erase(){
    /*
        删除行
    */
    for(int i = 0; i < table_ptr_->parm_num_; i++){
        //删除非主键索引, 释放内存空间
        switch(table_ptr_->parm_types_[i]){
            case __INT:
                //if(table_ptr_->index_tree_[i]!=NULL) table_ptr_->index_tree_[i]->DeleteData(new Index(*(int*)content_[i]));
                delete[](int *)content_[i];
                break;
            case __INT64:
                //if(table_ptr_->index_tree_[i]!=NULL) table_ptr_->index_tree_[i]->DeleteData(new Index(*(long long*)content_[i]));
                delete[](long long *)content_[i];
                break;
            case __REAL:
                //if(table_ptr_->index_tree_[i]!=NULL) table_ptr_->index_tree_[i]->DeleteData(new Index(*(double*)content_[i]));
                delete[](double *)content_[i];
                break;
            default:
                //if(table_ptr_->index_tree_[i]!=NULL) table_ptr_->index_tree_[i]->DeleteData(new Index((char*)content_[i]));
                delete[](char *)content_[i];
                break;
        }
    }
}


