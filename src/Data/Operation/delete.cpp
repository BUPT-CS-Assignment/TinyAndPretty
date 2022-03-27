#include<Data.h>
#include<Implement.h>

bool Table::DeleteValues(string conditions){
    int l = 0;
    //分离删除条件 暂时仅支持"="
    string* cond = Split(conditions,'=',l); 
    if(l == 0) return false;
    //定位删除元素名称
    int index = parameter_locate(cond[0]);
    if(index == -1) return false;
    Index* temp;
    //确定是否为主键
    if(index == prim_key_){
        //主键元素, 直接查找
        temp = new Index(cond[1],parm_types_[prim_key_]);
        return DeleteRow(*temp);
    }else{
        //非主键元素, 查询相应非主键索引, 回表
        if(index_tree_[index]==NULL) return delete_traversal(conditions);
        Index* sec_index = new Index(cond[1],parm_types_[index]);
        //查询非主键索引, 获得主键索引链表
        T_node<Index>* indexs = index_tree_[index]->getDataLink(sec_index);
        while(indexs!=NULL){
            //遍历链表, 删除所有元素
            DeleteRow(indexs->data);
            indexs = indexs->next;
        }
        return true;
    }
}

bool Table::delete_traversal(string conditions){
    cout<<"travel test"<<endl;
    return true;
}

bool Table::DeleteRow(Index& index){
    //查找插入数据页指针
    Page* page = new Page(this);
    Memorizer RAM;
    __uint16_t* page_offset =  pages_tree_->InsertLocate(&index);
    if(page_offset == NULL){
        cout<<"<E> DATA NOT FOUND"<<endl;
        return false;
    }
    RAM.PageLoad(*page_offset,this);
    if(!page->DeleteRow(index)) return false;
    Index temp = *(new Index(page->page_index_));
    if(page->cursor_pos_ == 0){
        if(*page_offset != total_pages_ - 1){
            add_empty_page(*page_offset);
        }
        RAM.PageFlush(*page_offset,this);
        pages_tree_->DeleteData(&temp);
    }else{
        RAM.PageStore(*page_offset,page);
    }
    RAM.TableUpdate(this);
    return true;
}
/*
bool Table::delete_page(Index& index){
    //查找插入数据页指针
    __uint16_t* page_offset =  pages_tree_->locate_data(&index);
    if(page_offset == NULL){
        cout<<"<E> DATA NOT FOUND"<<endl;
        return false;
    }
    Page* page = new Page(this);
    Memorizer M(this,page);
    M.page_read_in(*page_offset);
    Index temp = *(new Index(page->page_index_));
     pages_tree_->delete_data(&temp);
    if(page->__PageOffset != total_pages_){
        add_empty_page(page->__PageOffset);
    }
    M.page_flush(*page_offset);
    page->remove_page();
    M.update();
    return true;
}
*/
bool Table::remove_table(){/*
    for(int i = 0;i<__Cursor;i++){
         pages_tree_[i]->remove_page();
        delete[]  pages_tree_[i];
    }
    */
    delete[] parm_types_;
    delete[]  parm_names_;
    
    return true;
}

//删除指定行
bool Page::DeleteRow(Index& index){
    int p = -1;
    for(__uint16_t i = 0; i < cursor_pos_; i++){
        if(rows_[i]->getIndex() == index){
            p = i;
            break;
        }
    }
    if(p == -1){
        cout<<"<E> DATA NOT FOUND"<<endl;
        return false;
    }
    rows_[p]->Erase();
    for(__uint16_t i = p;i<cursor_pos_ -1;i++){
        rows_[i] = rows_[i+1];
    }
    cursor_pos_--;
    return true;
}


//删除整页
bool Page::remove_page(){
    for(__uint16_t i = 0;i<cursor_pos_;i++){
        rows_[i]->Erase();
    }
    cursor_pos_ = 0;
    return true;
}


void Row::Erase(){
    /*
        删除行
    */
    for(int i = 0;i<table_ptr_->parm_num_;i++){
        //删除非主键索引, 释放内存空间
        switch(table_ptr_->parm_types_[i]){
            case __INT :
                //if(table_ptr_->index_tree_[i]!=NULL) table_ptr_->index_tree_[i]->DeleteData(new Index(*(int*)content_[i]));
                delete[] (int*)content_[i];
                break;
            case __INT64 :
                //if(table_ptr_->index_tree_[i]!=NULL) table_ptr_->index_tree_[i]->DeleteData(new Index(*(long long*)content_[i]));
                delete[] (long long*)content_[i];
                break;
            case __REAL :
                //if(table_ptr_->index_tree_[i]!=NULL) table_ptr_->index_tree_[i]->DeleteData(new Index(*(double*)content_[i]));
                delete[] (double*)content_[i];
                break;
            default : 
                //if(table_ptr_->index_tree_[i]!=NULL) table_ptr_->index_tree_[i]->DeleteData(new Index((char*)content_[i]));
                delete[] (char*)content_[i];
                break;
        }
    }
}


