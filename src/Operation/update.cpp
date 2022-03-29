#include<Data.h>
#include<Implement.h>

//UPDATE table_name SET 'parm_name_1' = 'value_1', ... WHERE 'condition'

bool Table::UpdateValues(string condition, string setting){
    Analyzer CANZ(this), SANZ(this);
    CANZ.Extract(condition," AND ");
    SANZ.Extract(setting," , ");
    if(SANZ.KeySupport()){
        cout<<"<E> PRIM KEY CHANGE NOT ALLOWED"<<endl;
        return false;
    }
    if(CANZ.KeySupport()){
        return update_by_key(SANZ,CANZ);
    }
    return update_by_traverse(SANZ,CANZ);

}


bool Table::update_by_key(Analyzer& SANZ, Analyzer& CANZ){
    Index* index = CANZ.getCondVal(CANZ.getKeyPos());
    __uint16_t *offset = pages_tree_->InsertLocate(index);
    if(offset == NULL){
        cout << "<E> DATA NOT FOUND" << endl;
        return "";
    }
    Memorizer RAM;
    Page *page = RAM.PageLoad(*offset, this);
    if(!page->UpdateRow(SANZ,CANZ)){
        return false;
    }
    RAM.PageStore(*offset, page);
    return true;

}


bool Table::update_by_traverse(Analyzer& SANZ, Analyzer& CANZ){
    Node<__uint16_t, Index> *head = pages_tree_->getHead();
    if(head == NULL) return "";
    Memorizer RAM;
    while(head != NULL){
        for(int i = 0; i >= 0; i++){
            __uint16_t *page_offset = head->getData(i);
            if(page_offset == NULL) break;
            Page *page = RAM.PageLoad(*page_offset, this);
            if(!page->UpdateRow(SANZ,CANZ)){
                return false;
            }
            RAM.PageStore(*page_offset, page);
        }
        head = head->getNext();
    }
    return true;

}


bool Page::UpdateRow(Analyzer& SANZ, Analyzer& CANZ){
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





