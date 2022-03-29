#include<Data.h>
#include<Implement.h>


Analyzer::Analyzer(Table *table){
    table_ptr_ = table;
    cond_num = 0;
    cond_pos = new int[10];
    cond_cmp = new char[10];
    cond_val = new Index[10];
    parm_num = 0;
    key_pos = -1;
}

bool Analyzer::Extract(string conditions){
    if(conditions.length() == 0){
        cond_num = -1;
        return true;
    }
    while(true){
        if(conditions.length() == 0){
            return true;
        }
        size_t pos = conditions.find(" AND ");
        string temp;
        if(pos == conditions.npos){
            temp = conditions;
            conditions = "";
        }else{
            temp = conditions.substr(0, pos);
            conditions = conditions.substr(pos + 5, conditions.length() - pos - 5);
        }
        if(temp.find('=') != temp.npos){
            cond_cmp[cond_num] = '=';
        }
        else if(temp.find('<') != temp.npos){
            cond_cmp[cond_num] = '<';
        }
        else if(temp.find('>') != temp.npos){
            cond_cmp[cond_num] = '>';
        }
        int num = 0;
        string *str = Split(temp, cond_cmp[cond_num], num);
        cond_pos[cond_num] = table_ptr_->ParmLocate(str[0]);
        if(cond_pos[cond_num] == table_ptr_->prim_key_){
            key_pos = cond_num;
        }
        if(cond_pos[cond_num] == -1){
            cout<<"<E> PARAMETER NOT FOUND"<<endl;
            return false;
        }
        cond_val[cond_num] = *new Index(str[1], table_ptr_->parm_types_[cond_pos[cond_num]]);
        cond_num++;
    }
    return true;
}

bool Analyzer::Locate(string params){
    if(params == "*"){
        parm_num = -1;
        return true;
    }
    //int n;
    string* param = Split(params,',',parm_num);
    if(parm_num == 0) return false;
    parm_pos = new int[parm_num];
    for(int i = 0;i < parm_num; i++){
        parm_pos[i] = table_ptr_->ParmLocate(param[i]);
        if(parm_pos[i] == -1){
            cout << "<E> PARAMETER NOT FOUND" << endl;
            return false;
        }
    }
    return true;
}

bool Analyzer::Match(Row* row){
    if(cond_num == -1) return true;
    for(int i =0; i < cond_num; i++){
        DATA_TYPE type = table_ptr_->parm_types_[cond_pos[i]];
        Index temp;
        if(type == __INT){
            temp = Index(*(int*)row->content_[cond_pos[i]]);
        }else if(type == __INT64){
            temp = Index(*(long long*)row->content_[cond_pos[i]]);
        }else if(type == __REAL){
            temp = Index(*(double*)row->content_[cond_pos[i]]);
        }else if(type == __TEXT){
            temp = Index((char*)row->content_[cond_pos[i]], __TEXT);
        }else{
            temp = Index((char*)row->content_[cond_pos[i]], __LONGTEXT);
        }
        if(cond_cmp[i] == '='){
            if(temp == cond_val[i]){
                continue;
            }else{
                return false;
            }
        }else if(cond_cmp[i] == '<'){
            if(temp < cond_val[i]){
                continue;
            }else{
                return false;
            }
        }else if(cond_cmp[i] == '>'){
            if(temp > cond_val[i]){
                continue;
            }else{
                return false;
            }
        }
    }
    return true;
}