#include<data.h>
#include<implement.h>

void Table::DeleteValues(string condition){
    try{
        Analyzer ANZ(this);
        ANZ.Extract(condition, " and ");
        if(ANZ.KeySupport()){
            delete_by_key(ANZ);
        }else{
            delete_by_traverse(ANZ);
        }
    }catch(NEexception &e){
        throw e;
    }
}

void Table::delete_by_key(Analyzer &ANZ){
    try{
        Index *index = ANZ.getCondVal(ANZ.getKeyPos());
        DataNode<__uint16_t, Index> data_node = pages_tree_->LocateData(index);
        if(data_node.getData() == NULL){
            throw DATA_NOT_FOUND;
        }
        Memorizer RAM;
        Page *page;
        int cmp = ANZ.getCompareType(ANZ.getKeyPos());
        while(data_node.getData() != NULL){
            page = RAM.PageLoad(*data_node.getData(), this);
            page->DeleteRow(ANZ);
            if(page->cursor_pos_ == 0){
                page->Clear(*data_node.getData());
            }
            else{
                RAM.PageStore(*data_node.getData(), page);
            }
            if(cmp == 0) break;
            else if(cmp == -1) -- data_node;
            else ++ data_node;
        }
    }catch(NEexception &e){
        throw e;
    }
}


void Table::delete_by_traverse(Analyzer &ANZ){
    try{
        Memorizer RAM;
        DataNode<__uint16_t, Index> data_node = pages_tree_->getLink();
        while(data_node.getData() != NULL){
            __uint16_t *page_offset = data_node.getData();
            if(page_offset == NULL) break;
            Page *page = RAM.PageLoad(*page_offset, this);
            page->DeleteRow(ANZ);
            if(page->cursor_pos_ == 0){
                page->Clear(*page_offset);
            }
            else{
                RAM.PageStore(*page_offset, page);
                ++ data_node;
            }
            RAM.PageStore(*page_offset, page);
        }
    }catch(NEexception &e){
        throw e;
    }
}


void Page::Clear(__uint16_t offset){
    try{
        if(offset != table_ptr_->max_offset - 1){
            table_ptr_->add_empty_page(offset);
        }
        else{
            table_ptr_->max_offset --;
        }
        table_ptr_->pages_tree_->DeleteData(&page_index_);
        Memorizer RAM;
        RAM.PageFlush(offset, table_ptr_);
    }catch(NEexception &e){
        throw e;
    }
}

void Page::DeleteRow(Analyzer &ANZ){
    try{
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
    }catch(NEexception &e){
        throw e;
    }
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


