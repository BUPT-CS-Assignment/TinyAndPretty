#include<data.h>
#include<implement.h>

string Table::SelectValues(string param, string condition){
    try{
        Analyzer ANZ(this);
        ANZ.Locate(param);
        ANZ.Extract(condition, " and ");
        string res = *new string("");
        if(ANZ.KeySupport()){
            res = select_by_key(ANZ);
        }else{
            res = select_by_traverse(ANZ);
        }
        return "{"+res+"}";
    }catch(NEexception &e){
        throw e;
    }
    
}
string Table::select_by_key(Analyzer &ANZ){
    try{
        Index *index = ANZ.getCondVal(ANZ.getKeyPos());
        DataNode<__uint16_t, Index> data_node = pages_tree_->LocateData(index);
        if(data_node.getData() == NULL){
            return "";
        }
        Memorizer RAM;
        string res = *new string("");
        Page *page;
        int cmp = ANZ.getCompareType(ANZ.getKeyPos());
        while(data_node.getData() != NULL){
            page = RAM.PageLoad(*data_node.getData(), this);
            res = res + page->SelectRow(ANZ);
            if(cmp == 0) break;
            else if(cmp == -1) --data_node;
            else ++data_node;
        }
        res = res.substr(0, res.length() - 1);
        return res;
    }catch(NEexception &e){
        throw e;
    }
}


string Table::select_by_traverse(Analyzer &ANZ){
    //遍历搜索
    try{
        string res = *new string("");
        Memorizer RAM;
        DataNode<__uint16_t, Index> data_node = pages_tree_->getLink();
        while(data_node.getData() != NULL){
            __uint16_t *page_offset = data_node.getData();
            if(page_offset == NULL) break;
            Page *page = RAM.PageLoad(*page_offset, this);
            res = res + page->SelectRow(ANZ);
            ++ data_node;
        }
        if(res.length() > 1){
            res = res.substr(0, res.length() - 1);
        }
        return res;
    }catch(NEexception &e){
        throw e;
    }
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


string Page::SelectRow(Analyzer &ANZ){
    string str = *new string("");
    for(int i = 0; i < cursor_pos_; i++){
        if(ANZ.Match(rows_[i])){
            str = str + rows_[i]->get_values(ANZ.getParmPos(), ANZ.getParmNum()) + ",";
        }
    }
    return str;
}



