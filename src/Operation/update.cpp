#include<Data.h>
#include<Implement.h>

//UPDATE table_name SET 'parm_name_1' = 'value_1', ... WHERE 'condition'

bool Table::UpdateValues(string condition, string value){
    if(value.length() == 0){
        return false;
    }
    int value_num = -1, cond_num = -1, cond_pos = -1;
    string *states = Split(value, ',', value_num);
    int value_pos[value_num] = {0};
    string values[value_num];
    for(int i = 0; i < value_num; i++){
        int temp2;
        string *temp = Split(states[i], '=', temp2);
        if(temp2 != 2){
            cout << "<E> PARAMETER FROMAT MISMATCH" << endl;
            return false;
        }
        value_pos[i] = ParmLocate(temp[0]);
        if(value_pos[i] == prim_key_){
            cout << "<E> PRIM KEY CHANGE NOT ALLOWED" << endl;
            return false;
        }
        values[i] = temp[1];
    }
    string *conds = NULL;
    if(condition.length() != 0){
        conds = Split(condition, '=', cond_num);
        if(cond_num != 2) return "";
        cond_pos = ParmLocate(conds[0]);
    }
    if(cond_pos == prim_key_){
        return update_by_key(value_pos, values, value_num, prim_key_, conds[1]);
    }
    else if(cond_pos == -1){
        return update_by_traverse(value_pos, values, value_num, -1, "");
    }
    else{
        return update_by_traverse(value_pos, values, value_num, cond_pos, conds[1]);
    }

}

bool Table::update_by_key(int value_pos[], string values[], int value_num, int cond_pos, string cond_value){
    Index *index = new Index(cond_value, getKeyType());
    __uint16_t *offset = pages_tree_->InsertLocate(index);
    if(offset == NULL){
        cout << "<E> DATA NOT FOUND" << endl;
        return "";
    }
    Memorizer RAM;
    Page *page = RAM.PageLoad(*offset, this);
    if(!page->UpdateRow(value_pos, values, value_num, cond_pos, cond_value)){
        return false;
    }
    RAM.PageStore(*offset, page);
    return true;
}

bool Table::update_by_traverse(int value_pos[], string values[], int value_num, int cond_pos, string cond_value){
    //遍历搜索
    Node<__uint16_t, Index> *head = pages_tree_->getHead();
    if(head == NULL) return "";
    Memorizer RAM;
    while(head != NULL){
        for(int i = 0; i >= 0; i++){
            __uint16_t *page_offset = head->getData(i);
            if(page_offset == NULL) break;
            Page *page = RAM.PageLoad(*page_offset, this);
            if(!page->UpdateRow(value_pos, values, value_num, cond_pos, cond_value)){
                return false;
            }
            RAM.PageStore(*page_offset, page);
        }
        head = head->getNext();
    }
    return true;
}

bool Page::UpdateRow(int value_pos[], string values[], int value_num, int cond_pos, string cond_value){
    if(cond_pos == -1){
        for(int i = 0; i < cursor_pos_; i++){
            if(!rows_[i]->update_values(value_pos, values, value_num)){
                return false;
            }
        }
    }
    else if(cond_pos == table_ptr_->prim_key_){
        for(int i = 0; i < cursor_pos_; i++){
            if(rows_[i]->get_value(cond_pos) == cond_value){
                return rows_[i]->update_values(value_pos, values, value_num);
            }
        }
        return false;
    }
    else{
        for(int i = 0; i < cursor_pos_; i++){
            if(rows_[i]->get_value(cond_pos) == cond_value){
                if(!rows_[i]->update_values(value_pos, values, value_num)){
                    return false;
                }
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





