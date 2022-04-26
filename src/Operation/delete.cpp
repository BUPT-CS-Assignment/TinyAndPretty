#include<Basic/data.h>
#include<Utils/implement.h>
#include<Basic/process.h>
using namespace std;

void Table::DeleteValues(string condition){
    try{
        if(!StatusCheck(table_status_,SIG_FREE,SIG_CHECK_TIMES)){
            throw ACTION_BUSY;
        }
        table_status_ = SIG_RUN;
        Analyzer ANZ(this);
        ANZ.Extract(condition, " and ");
        if(ANZ.KeySupport()){
            delete_by_key(ANZ);
        }
        else{
            delete_by_traverse(ANZ);
        }
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

void Table::delete_by_key(Analyzer& ANZ){
    try{

        Index *index = ANZ.getCondVal(ANZ.getKeyPos());
        if(index == NULL){
            throw SYSTEM_ERROR;
        }
        DataNode<__uint16_t, Index> data_node = pages_tree_->LocateData(*index);
        Memorizer RAM(this);
        Page* page;
        int cmp = ANZ.getCompareType(ANZ.getKeyPos());
        if(data_node.getData() == NULL){
            if(cmp == 1)data_node = pages_tree_->getLink();
            else throw DATA_NOT_FOUND;
        }
        while(data_node.getData() != NULL){
            page = RAM.PageLoad(*data_node.getData());
            page->DeleteRow(ANZ);
            if(page->cursor_pos_ == 0){
                page->Clear(*data_node.getData());
            }
            else{
                RAM.PageStore(*data_node.getData(), page);
            }
            /////
            if(cmp == 0 || ANZ.stop_flag == 2){
                if(page->cursor_pos_ == 0)  pages_tree_->DeleteData(page->page_index_);
                break;
            }
            else{
                if(cmp == -1) --data_node;
                else if(cmp == 1 && page->cursor_pos_ != 0) ++data_node;
                //考虑节点合并对迭代器影响
                if(page->cursor_pos_ == 0){
                    //记录当前节点左节点的值
                    Node<uint16_t, Index>* temp = (data_node.node == NULL ? NULL : data_node.node->get_side(0));
                    pages_tree_->DeleteData(page->page_index_);    //从索引树中删除该页索引
                    if(temp != NULL && data_node.node->get_cursor() == 0){
                        data_node.node = temp;
                        data_node.pos = temp->get_cursor() + (cmp == 1 ? data_node.pos : - 1);
                    }
                }
            }
            page->Erase();
        }
    }
    catch(NEexception& e){
        throw e;
    }
    catch(exception& e){
        throw SYSTEM_ERROR;
    }
}


void Table::delete_by_traverse(Analyzer& ANZ){
    try{
        Memorizer RAM(this);
        DataNode<__uint16_t, Index> data_node = pages_tree_->getLink();
        while(data_node.getData() != NULL){
            __uint16_t* page_offset = data_node.getData();
            if(page_offset == NULL) break;
            Page* page = RAM.PageLoad(*page_offset);
            page->DeleteRow(ANZ);
            if(page->cursor_pos_ == 0){
                page->Clear(*page_offset);
            }
            else{
                RAM.PageStore(*page_offset, page);
            }
            if(page->cursor_pos_ != 0)  ++ data_node;
            else if(page->cursor_pos_ == 0){
                //记录当前节点左节点的值
                Node<uint16_t, Index>* temp = (data_node.node == NULL ? NULL : data_node.node->get_side(0));
                pages_tree_->DeleteData(page->page_index_);    //从索引树中删除该页索引
                if(temp != NULL && data_node.node->get_cursor() == 0){
                    data_node.node = temp;
                    data_node.pos = temp->get_cursor() + data_node.pos;
                }
            }
            page->Erase();
        }

    }
    catch(NEexception& e){
        throw e;
    }
    catch(exception& e){
        throw SYSTEM_ERROR;
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
        Memorizer RAM(table_ptr_);
        RAM.PageFlush(offset);
    }
    catch(NEexception& e){
        throw e;
    }
    catch(exception& e){
        throw SYSTEM_ERROR;
    }
}

void Page::DeleteRow(Analyzer& ANZ){
    try{
        if(rows_ == NULL){
            throw SYSTEM_ERROR;
        }
        if(ANZ.KeySupport() && ANZ.getCompareType(ANZ.getKeyPos()) == -1){
            //递减顺序筛选
            
            for(int i = cursor_pos_ - 1; i >= 0; i--){
                if(ANZ.Match(rows_[i])){
                    table_ptr_->db_->AddCount();
                    rows_[i]->Erase();
                    for(int j = i; j < cursor_pos_ - 1; j++){
                        rows_[j] = rows_[j + 1];
                    }
                    rows_[cursor_pos_ - 1] = NULL;
                    cursor_pos_--;
                }
                else{
                    if(ANZ.stop_flag == 2){
                        return; //超出主键筛选范围, 返回
                    }
                }
            }
        }
        else{
            //递增顺序筛选
            for(int i = 0; i < cursor_pos_; i++){
                if(ANZ.Match(rows_[i])){
                    table_ptr_->db_->AddCount();
                    rows_[i]->Erase();
                    for(int j = i; j < cursor_pos_ - 1; j++){
                        rows_[j] = rows_[j + 1];
                    }
                    rows_[cursor_pos_ - 1] = NULL;
                    cursor_pos_--;
                    i--;
                }
                else{
                    if(ANZ.stop_flag == 2){
                        return; //超出主键筛选范围, 返回
                    }
                }
            }
        }
        ANZ.stop_flag = 1;
    }
    catch(NEexception& e){
        throw e;
    }
    catch(exception& e){
        throw SYSTEM_ERROR;
    }
}



