#include<Data.h>
#include<Implement.h>

bool Table::DeleteValues(string condition){
    Analyzer ANZ(this);
    ANZ.Extract(condition," AND ");
    if(ANZ.KeySupport()){
        return delete_by_key(ANZ);
    }
    return delete_by_traverse(ANZ);
}

bool Table::delete_by_key(Analyzer& ANZ){
    Index* index = ANZ.getCondVal(ANZ.getKeyPos());
    __uint16_t *offset = pages_tree_->InsertLocate(index);
    if(offset == NULL){
        cout << "<E> DATA NOT FOUND" << endl;
        return "";
    }
    Memorizer RAM;
    Page *page = RAM.PageLoad(*offset, this);
    if(!page->DeleteRow(ANZ)){
        return false;
    }
    if(page->cursor_pos_ == 0){
        page->Clear(*offset);
    }
    else{
        RAM.PageStore(*offset, page);
    }
    return true;
}


bool Table::delete_by_traverse(Analyzer& ANZ){
    Node<__uint16_t, Index> *head = pages_tree_->getHead();
    if(head == NULL) return "";
    Memorizer RAM;
    while(head != NULL){
        for(int i = 0; i >= 0; i++){
            __uint16_t *page_offset = head->getData(i);
            if(page_offset == NULL) break;
            Page *page = RAM.PageLoad(*page_offset, this);
            if(!page->DeleteRow(ANZ)){
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

bool Page::DeleteRow(Analyzer& ANZ){
    for(int i = 0; i < cursor_pos_; i++){
        if(ANZ.Match(rows_[i])){
            rows_[i]->Erase();
            for(int j = i; j < cursor_pos_ - 1; j++){
                rows_[j] = rows_[j + 1];
            }
            cursor_pos_--;
            i--;
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


