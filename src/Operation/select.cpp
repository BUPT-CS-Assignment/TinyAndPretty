#include<Basic/data.h>
#include<Utils/implement.h>
#include<Basic/process.h>
using namespace std;

string Table::SelectValues(string param, string condition){
    try{
        Analyzer ANZ(this);
        ANZ.Locate(param);
        ANZ.Extract(condition, " and ");
        string res = "";
        /*
        if(__LockCheck__(table_lock_,SIG_CHECK_TIMES)!=SIG_UNLOCK){
            throw ACTION_BUSY;
        }
        table_lock_ = SIG_LOCK;
        */
        if(ANZ.KeySupport()){
            res = select_by_key(ANZ);
        }
        else{
            res = select_by_traverse(ANZ);
        }
        /* set header */
        string head;
        if(param == "*"){
            for(int i = 0; i < parm_num_; i++){
                head += string(parm_names_[i]);
                head += (i == parm_num_ - 1 ? ";" : ",");
            }
        }
        else{
            head = param + ";";
        }
        //table_lock_ = SIG_UNLOCK;
        return head+res;
    }
    catch(NEexception &e){
        //table_lock_ = SIG_UNLOCK;
        throw e;
    }
    catch(exception &e){
        //table_lock_ = SIG_UNLOCK;
        throw SYSTEM_ERROR;
    }

}
string Table::select_by_key(Analyzer &ANZ){
    try{
        Index *index = ANZ.getCondVal(ANZ.getKeyPos());
        DataNode<__uint16_t, Index> data_node = pages_tree_->LocateData(index);
        int cmp = ANZ.getCompareType(ANZ.getKeyPos());
        if(data_node.getData() == NULL){
            if(cmp == 1)data_node = pages_tree_->getLink();
            else return "";
        }
        Memorizer RAM(this);
        string res = "";
        Page *page;
        while(data_node.getData() != NULL){
            page = RAM.PageLoad(*data_node.getData());
            string temp = page->SelectRow(ANZ);
            if(cmp == -1) res = temp + res;
            else res = res + temp;
            if(cmp == 0 || ANZ.stop_flag == 2) break;
            else if(cmp == -1){
                --data_node;
            }
            else ++data_node;
            page->Erase();
        }
        res = res.substr(0, res.length() - 1);
        return res;
    }
    catch(NEexception &e){
        throw e;
    }
    catch(exception &e){
        throw SYSTEM_ERROR;
    }
}


string Table::select_by_traverse(Analyzer &ANZ){
    //遍历搜索
    try{
        string res = "";
        Memorizer RAM(this);
        DataNode<__uint16_t, Index> data_node = pages_tree_->getLink();
        while(data_node.getData() != NULL){
            __uint16_t *page_offset = data_node.getData();
            if(page_offset == NULL) break;
            Page *page = RAM.PageLoad(*page_offset);
            res = res + page->SelectRow(ANZ);
            ++ data_node;
            page->Erase();
        }
        if(res.length() > 1){
            res = res.substr(0, res.length() - 1);
        }
        return res;
    }
    catch(NEexception &e){
        throw e;
    }
    catch(exception &e){
        throw SYSTEM_ERROR;
    }
}


string Row::get_values(int values[], int n){
    string str = "";
    if(n == -1){
        for(int i = 0; i < table_ptr_->parm_num_; i++){
            str = str + get_value(i) + (i == table_ptr_->parm_num_ - 1 ? "" : ",");
        }
    }
    else{
        for(int i = 0; i < n; i++){
            str = str + get_value(values[i]) + (i == n - 1 ? "" : ",");
        }
    }
    return str;
}


string Row::get_value(int i){
    DATA_TYPE type = table_ptr_->parm_types_[i];
    string str = "";
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
    string str = "";
    if(ANZ.KeySupport() && ANZ.getCompareType(ANZ.getKeyPos()) == -1){
        //递减顺序筛选
        for(int i = cursor_pos_ - 1; i >= 0; i--){
            if(ANZ.Match(rows_[i])){
                table_ptr_->db_->AddCount();
                str = rows_[i]->get_values(ANZ.getParmPos(), ANZ.getParmNum()) + ";" + str;
            }
            else{
                if(ANZ.stop_flag == 2){
                    return str;//超出主键筛选范围, 返回
                }
            }
        }
    }
    else{
        //递增顺序筛选
        for(int i = 0; i < cursor_pos_; i++){
            if(ANZ.Match(rows_[i])){
                table_ptr_->db_->AddCount();
                str = str + rows_[i]->get_values(ANZ.getParmPos(), ANZ.getParmNum()) + ";";
            }
            else{
                if(ANZ.stop_flag == 2){
                    return str; //超出主键筛选范围, 返回
                }
            }
        }
    }
    ANZ.stop_flag = 1;
    return str;
}



