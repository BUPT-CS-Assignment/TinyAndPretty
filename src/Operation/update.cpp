#include<data.h>
#include<implement.h>

//UPDATE table_name SET 'parm_name_1' = 'value_1', ... WHERE 'condition'

void Table::UpdateValues(string condition, string setting){
    try{
        Analyzer CANZ(this), SANZ(this);
        CANZ.Extract(condition, " and ");
        SANZ.Extract(setting, " , ");
        if(SANZ.KeySupport()){
            throw KEY_VAL_CHANGE_NOT_ALLOWED;
        }
        if(CANZ.KeySupport()){
            update_by_key(SANZ, CANZ);
        }
        else{
            update_by_traverse(SANZ, CANZ);
        }
    }
    catch(NEexception &e){
        throw e;
    }

}


void Table::update_by_key(Analyzer &SANZ, Analyzer &CANZ){
    try{
        Index *index = CANZ.getCondVal(CANZ.getKeyPos());
        DataNode<__uint16_t, Index> data_node = pages_tree_->LocateData(index);
        if(data_node.getData() == NULL){
            throw DATA_NOT_FOUND;
        }
        Memorizer RAM;
        Page *page;
        int cmp = CANZ.getCompareType(CANZ.getKeyPos());
        while(data_node.getData() != NULL){
            page = RAM.PageLoad(*data_node.getData(), this);
            page->UpdateRow(SANZ, CANZ);
            RAM.PageStore(*data_node.getData(), page);
            if(cmp == 0 || CANZ.stop_flag == 2) break;
            else if(cmp == -1) --data_node;
            else ++data_node;
        }
    }
    catch(NEexception &e){
        throw e;
    }
}


void Table::update_by_traverse(Analyzer &SANZ, Analyzer &CANZ){
    try{
        Memorizer RAM;
        DataNode<__uint16_t, Index> data_node = pages_tree_->getLink();
        while(data_node.getData() != NULL){
            __uint16_t *page_offset = data_node.getData();
            if(page_offset == NULL) break;
            Page *page = RAM.PageLoad(*page_offset, this);
            page->UpdateRow(SANZ, CANZ);
            RAM.PageStore(*page_offset, page);
            ++ data_node;
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
    }
}





