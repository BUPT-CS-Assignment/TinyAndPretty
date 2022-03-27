#include<Data.h>
#include<Implement.h>

string Table::SelectValues(string condition,string value){
    //条件解析
    int cond_num = -1, cond_pos = -1, value_num = -1;
    string* values = NULL;
    int* value_pos = NULL;
    string* conds = NULL;
    if(condition.length()!=0){
        conds = Split(condition,'=',cond_num);
        if(cond_num != 2) return "";
        cond_pos = parameter_locate(conds[0]);
    }
    if(value == "*"){
        value_num = parm_num_;
    }else{
        values = Split(value,',',value_num);
        value_pos = new int[value_num]{0};
        for(int i = 0; i < value_num; i++){
            value_pos[i] = parameter_locate(values[i]);
            if(value_pos[i] == -1){
                cout<<"<E> NO SUCH PARAMETER"<<endl;
                return "";
            }
        }
    }
    if(cond_pos == prim_key_){
        return select_by_key(value_pos,value_num,prim_key_,conds[1]);
    }else if(cond_pos == -1){
        return select_by_traverse(value_pos,value_num,-1,"");
    }else{
        return select_by_traverse(value_pos,value_num,cond_pos,conds[1]);
    }
}


Row* Page::SelectRow(Index& index){
    if(cursor_pos_ == 0) return NULL;
    if(index < rows_[0]->row_index_) return NULL;
    for(int i = 0; i < cursor_pos_; i++){
        if(index == rows_[i]->row_index_){
            return rows_[i];
        }
        if(index < rows_[i]->row_index_) return NULL;
    }
    return NULL;
}

string Table::select_by_traverse(int values[],int value_num,int cond_pos,string cond_value){
    //遍历搜索
    Node<__uint16_t,Index>* head =  pages_tree_->getHead();
    if(head == NULL) return "";
    string res = *new string("{");
    Memorizer RAM;
    while(head != NULL){
        for(int i = 0; i >= 0 ; i++){
            __uint16_t* page_offset =  head->getData(i);
            if(page_offset == NULL) break;
            Page* page = RAM.PageLoad(*page_offset,this);
            res = res + page->select_check_all(values,value_num,cond_pos,cond_value);
        }
        head = head->getNext();
    }
    res = res.substr(0,res.length()-1);
    return res+"}";
}



string Page::select_check_all(int values[],int value_num,int cond_pos,string cond_value){
    //页内搜索
    string str = *new string("");
    if(cond_pos == -1){
        for(int i = 0; i < cursor_pos_; i++){
            str = str + rows_[i]->get_values(values,value_num) + ",";
        }
    }else{
        for(int i = 0; i < cursor_pos_; i++){
            if(rows_[i]->get_value(cond_pos) == cond_value){
                str = str + rows_[i]->get_values(values,value_num) + ",";
            }
        }
    }
    return str;
}


string Table::select_by_key(int values[],int value_num,int cond_pos,string cond_value){
    Index* index = new Index(cond_value,getKeyType());
    __uint16_t* offset = pages_tree_->InsertLocate(index);
    if(offset == NULL){
        cout<<"<E> DATA NOT FOUND"<<endl;
        return "";
    }
    Memorizer RAM;
    Page* page = RAM.PageLoad(*offset,this);
    Row* row = page->SelectRow(*index);
    if(row == NULL){
        cout<<"<E> DATA NOT FOUND"<<endl;
        return "";
    }
    return row->get_values(values,value_num);
}




string Row::get_value(int i){
    DATA_TYPE type = table_ptr_->parm_types_[i];
    string str = *new string("");
    switch(type){
        case __INT :
            str = to_string(*((int*)content_[i]));
            break;
        case __INT64 :
            str = to_string(*((long long*)content_[i]));
            break;
        case __REAL :
            str = to_string(*((double*)content_[i]));
            break;
        default:
            str = (char*)content_[i];
            break;
    }
    return str;
}

string Row::get_values(int values[],int n){
    string str = *new string("{");
    if(n==table_ptr_->parm_num_){
        for(int i = 0; i < table_ptr_->parm_num_; i++){
            str = str + get_value(i) + (i == table_ptr_->parm_num_ -1 ? "}":",");
        }
    }else{
        for(int i  = 0; i < n; i++){
            str = str + get_value(values[i]) + (i == n-1 ? "}":",");
        }
    }
    return str;
}








