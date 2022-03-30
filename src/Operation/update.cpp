#include<Data.h>
#include<Implement.h>

//UPDATE table_name SET 'parm_name_1' = 'value_1', ... WHERE 'condition'

bool Table::UpdateValues(string condition, string setting){
    Analyzer CANZ(this), SANZ(this);
    CANZ.Extract(condition, " and ");
    SANZ.Extract(setting, " , ");
    if(SANZ.KeySupport()){
        cout << "<E> PRIM KEY CHANGE NOT ALLOWED" << endl;
        return false;
    }
    if(CANZ.KeySupport()){
        return update_by_key(SANZ, CANZ);
    }
    return update_by_traverse(SANZ, CANZ);

}


bool Table::update_by_key(Analyzer &SANZ, Analyzer &CANZ){
    Index *index = CANZ.getCondVal(CANZ.getKeyPos());
    DataNode<__uint16_t, Index> data_node = pages_tree_->LocateData(index);
    if(data_node.getData() == NULL){
        cout << "<E> DATA NOT FOUND" << endl;
        return "";
    }
    Memorizer RAM;
    Page *page;
    int cmp = CANZ.getCompareType(CANZ.getKeyPos());
    while(data_node.getData() != NULL){
        page = RAM.PageLoad(*data_node.getData(), this);
        if(!page->UpdateRow(SANZ, CANZ)){
            return false;
        }
        if(cmp == 0) break;
        else if(cmp == -1) --data_node;
        else ++data_node;
    }
    return true;
}


bool Table::update_by_traverse(Analyzer &SANZ, Analyzer &CANZ){
    Memorizer RAM;
    DataNode<__uint16_t, Index> data_node = pages_tree_->getLink();
    while(data_node.getData() != NULL){
        __uint16_t *page_offset = data_node.getData();
        if(page_offset == NULL) break;
        Page *page = RAM.PageLoad(*page_offset, this);
        if(!page->UpdateRow(SANZ, CANZ)){
            return false;
        }
        RAM.PageStore(*page_offset, page);
        ++ data_node;
    }
    return true;
}


bool Page::UpdateRow(Analyzer &SANZ, Analyzer &CANZ){
    for(int i = 0; i < cursor_pos_; i++){
        if(CANZ.Match(rows_[i])){
            if(!rows_[i]->update_values(SANZ.getCondPos(), SANZ.getCondOrigin(), SANZ.getCondNum())){
                return false;
            }
        }
    }
    return true;
}


bool Row::update_values(int value_pos[], string values[], int n){
    for(int i = 0; i < n; i++){
        if(!update_value(value_pos[i], values[i])){
            return false;
        }
    }
    return true;
}





