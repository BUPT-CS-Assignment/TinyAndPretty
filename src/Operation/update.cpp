#include<Basic/data.h>
#include<Utils/implement.h>
#include<Basic/process.h>
using namespace std;

//UPDATE table_name SET 'parm_name_1' = 'value_1', ... WHERE 'condition'

void Table::UpdateValues(string condition, string setting){
    try{
        Analyzer CANZ(this), SANZ(this);
        CANZ.Extract(condition, " and ");
        SANZ.Extract(setting, " , ");
        /* Lock Check */
        if(!StatusCheck(table_status_,SIG_FREE,SIG_CHECK_TIMES)){
            throw ACTION_BUSY;
        }
        table_status_ = SIG_RUN;
        if(SANZ.KeySupport()){
            throw KEY_VAL_CHANGE_NOT_ALLOWED;
        }
        if(CANZ.KeySupport()){
            update_by_key(SANZ, CANZ);
        }
        else{
            update_by_traverse(SANZ, CANZ);
        }
        table_status_ = SIG_FREE;
    }
    catch(NEexception &e){
        table_status_ = SIG_FREE;
        throw e;
    }

}


void Table::update_by_key(Analyzer &SANZ, Analyzer &CANZ){
    try{
        Index* index = CANZ.getCondVal(CANZ.getKeyPos());
        if(index == NULL){
            throw SYSTEM_ERROR;
        }
        DataNode<__uint16_t, Index> data_node = pages_tree_->LocateData(*index);
        int cmp = CANZ.getCompareType(CANZ.getKeyPos());
        if(data_node.getData() == NULL){
            if(cmp == 1)data_node = pages_tree_->getLink();
            else throw DATA_NOT_FOUND;
        }
        Memorizer RAM(this);
        Page *page;
        while(data_node.getData() != NULL){
            page = RAM.PageLoad(*data_node.getData());
            page->UpdateRow(SANZ, CANZ);
            RAM.PageStore(*data_node.getData(), page);
            if(cmp == 0 || CANZ.stop_flag == 2) break;
            else if(cmp == -1) --data_node;
            else ++data_node;
            page->Erase();
        }
    }
    catch(NEexception &e){
        throw e;
    }
}


void Table::update_by_traverse(Analyzer &SANZ, Analyzer &CANZ){
    try{
        Memorizer RAM(this);
        DataNode<__uint16_t, Index> data_node = pages_tree_->getLink();
        while(data_node.getData() != NULL){
            __uint16_t* page_offset = data_node.getData();
            if(page_offset == NULL) break;
            Page *page = RAM.PageLoad(*page_offset);
            page->UpdateRow(SANZ, CANZ);
            RAM.PageStore(*page_offset, page);
            ++ data_node;
            page->Erase();
        }
    }
    catch(NEexception &e){
        throw &e;
    }
}


void Page::UpdateRow(Analyzer &SANZ, Analyzer &CANZ){
    try{
        if(CANZ.KeySupport() && CANZ.getCompareType(CANZ.getKeyPos()) == -1){
            //递减顺序筛选
            for(int i = cursor_pos_ - 1; i >= 0; i--){
                if(CANZ.Match(rows_[i])){
                    rows_[i]->update_values(SANZ.getCondPos(), SANZ.getCondOrigin(), SANZ.getCondNum());
                }
                else{
                    if(CANZ.stop_flag == 2){
                        return; //超出主键筛选范围, 返回
                    }
                }
            }
        }
        else{
            //递增顺序筛选
            for(int i = 0; i < cursor_pos_; i++){
                if(CANZ.Match(rows_[i])){
                    rows_[i]->update_values(SANZ.getCondPos(), SANZ.getCondOrigin(), SANZ.getCondNum());
                }
                else{
                    if(CANZ.stop_flag == 2){
                        return; //超出主键筛选范围, 返回
                    }
                }
            }
        }
        CANZ.stop_flag = 1;
    }
    catch(NEexception &e){
        throw e;
    }catch(exception &e){
        throw SYSTEM_ERROR;
    }
}


void Row::update_values(int value_pos[], string values[], int n){
    try{
        for(int i = 0; i < n; i++){
            update_value(value_pos[i], values[i]);
        }
    }
    catch(NEexception &e){
        throw e;
    }catch(exception &e){
        throw SYSTEM_ERROR;
    }
}





