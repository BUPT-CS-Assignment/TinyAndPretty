#include<Data.h>
#include<Implement.h>

string Table::SelectValues(string param, string condition){
    Analyzer ANZ(this);
    ANZ.Locate(param);
    ANZ.Extract(condition," AND ");
    if(ANZ.KeySupport()){
        return select_by_key(ANZ);
    }
    return select_by_traverse(ANZ);

}
string Table::select_by_key(Analyzer& ANZ){
    Index* index = ANZ.getCondVal(ANZ.getKeyPos());
    __uint16_t *offset = pages_tree_->InsertLocate(index);
    if(offset == NULL){
        cout << "<E> DATA NOT FOUND" << endl;
        return "";
    }
    Memorizer RAM;
    Page *page = RAM.PageLoad(*offset, this);
    return "{"+ page->SelectRow(ANZ) + "}";
}


string Table::select_by_traverse(Analyzer& ANZ){
    //遍历搜索
    Node<__uint16_t, Index> *head = pages_tree_->getHead();
    if(head == NULL) return "";
    string res = *new string("{");
    Memorizer RAM;
    while(head != NULL){
        for(int i = 0; i >= 0; i++){
            __uint16_t *page_offset = head->getData(i);
            if(page_offset == NULL) break;
            Page *page = RAM.PageLoad(*page_offset, this);
            res = res + page->SelectRow(ANZ);
        }
        head = head->getNext();
    }
    if(res.length() > 1){
        res = res.substr(0, res.length() - 1);
    }
    return res + "}";
}


string Row::get_values(int values[], int n){
    string str = *new string("{");
    if(n == -1){
        for(int i = 0; i < table_ptr_->parm_num_; i++){
            str = str + get_value(i) + (i == table_ptr_->parm_num_ - 1 ? "}" : ",");
        }
    }
    else{
        for(int i = 0; i < n; i++){
            str = str + get_value(values[i]) + (i == n - 1 ? "}" : ",");
        }
    }
    return str;
}


string Row::get_value(int i){
    DATA_TYPE type = table_ptr_->parm_types_[i];
    string str = *new string("");
    switch(type){
        case __INT:
            str = to_string(*((int *)content_[i]));
            break;
        case __INT64:
            str = to_string(*((long long *)content_[i]));
            break;
        case __REAL:
            str = to_string(*((double *)content_[i]));
            break;
        default:
            str = (char *)content_[i];
            break;
    }
    return str;
}


string Page::SelectRow(Analyzer& ANZ){
    string str = *new string("");
    for(int i = 0; i < cursor_pos_; i++){
        if(ANZ.Match(rows_[i])){
            str = str + rows_[i]->get_values(ANZ.getParmPos(), ANZ.getParmNum()) + ",";
        }
    }
    return str;
}



