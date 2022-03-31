#include<data.h>
#include<implement.h>


Analyzer::Analyzer(Table *table){
    table_ptr_ = table;
    cond_num = 0;
    cond_pos = new int[10];
    cond_cmp = new char[10];
    cond_val = new Index[10];
    cond_origin = new string[10];
    parm_num = 0;
    key_pos = -1;
    stop_flag = 0;  
    key_range[0] = key_range[1] = NULL;
}

int Analyzer::getCompareType(int pos){
    if(pos >= cond_num) return -2;
    if(cond_cmp[pos] == '=') return 0;
    if(cond_cmp[pos] == '<') return -1;
    if(cond_cmp[pos] == '>') return 1;
    return -2;
}

void Analyzer::Extract(string conditions, string pattern){
    if(conditions.length() == 0){
        cond_num = -1;
        return;
    }
    key_range[0] = key_range[1] = NULL;
    int l = pattern.length();
    while(true){
        if(conditions.length() == 0){
            return;
        }
        size_t pos = conditions.find(pattern);
        string temp;
        if(pos == conditions.npos){
            temp = conditions;
            conditions = "";
        }
        else{
            temp = conditions.substr(0, pos);
            conditions = conditions.substr(pos + l, conditions.length() - pos - l);
        }
        //分离操作符
        if(temp.find('=') != temp.npos){
            cond_cmp[cond_num] = '=';
        }
        else if(temp.find('<') != temp.npos){
            cond_cmp[cond_num] = '<';
        }
        else if(temp.find('>') != temp.npos){
            cond_cmp[cond_num] = '>';
        }
        else{
            throw COMPARE_NONSUPPORT;
        }
        //比值存入
        int num = 0;
        string *str = Split(temp, cond_cmp[cond_num], num);
        cond_pos[cond_num] = table_ptr_->ParmLocate(str[0]);
        if(cond_pos[cond_num] == -1){
            throw PARAM_NOT_FOUND;
        }
        cond_origin[cond_num] = str[1];
        cond_val[cond_num] = *new Index(str[1], table_ptr_->parm_types_[cond_pos[cond_num]]);
        /////////////////////
        if(cond_pos[cond_num] == -1){
            throw PARAM_NOT_FOUND;
        }
        cond_origin[cond_num] = str[1];
        cond_val[cond_num] = *new Index(str[1], table_ptr_->parm_types_[cond_pos[cond_num]]);
        if(cond_pos[cond_num] == table_ptr_->prim_key_){
            ///更新主键范围
            if(cond_cmp[cond_num] == '>'){
                if(key_range[0] == NULL || *key_range[0] < cond_val[cond_num]){
                    key_pos = cond_num;
                    key_range[0] = &cond_val[cond_num];
                }
            }
            else if(cond_cmp[cond_num] == '<'){
                if(key_range[1] == NULL || *key_range[1] > cond_val[cond_num]){
                    key_pos = cond_num;
                    key_range[1] = &cond_val[cond_num];
                }
            }else{
                key_pos = cond_num;
            }
        }
        cond_num++;
    }
}

void Analyzer::Locate(string params){
    if(params == "*"){
        parm_num = -1;
        return;
    }
    //int n;
    string *param = Split(params, ',', parm_num);
    if(parm_num == 0){
        throw PARAM_EMPTY;
    }
    parm_pos = new int[parm_num];
    for(int i = 0; i < parm_num; i++){
        parm_pos[i] = table_ptr_->ParmLocate(param[i]);
        if(parm_pos[i] == -1){
            throw PARAM_NOT_FOUND;
        }
    }
}

bool Analyzer::Match(Row *row){
    if(cond_num == -1) return true;
    for(int i = 0; i < cond_num; i++){
        DATA_TYPE type = table_ptr_->parm_types_[cond_pos[i]];
        Index temp;
        if(type == __INT){
            temp = Index(*(int *)row->content_[cond_pos[i]]);
        }
        else if(type == __INT64){
            temp = Index(*(long long *)row->content_[cond_pos[i]]);
        }
        else if(type == __REAL){
            temp = Index(*(double *)row->content_[cond_pos[i]]);
        }
        else if(type == __TEXT){
            temp = Index((char *)row->content_[cond_pos[i]], __TEXT);
        }
        else{
            temp = Index((char *)row->content_[cond_pos[i]], __LONGTEXT);
        }
        if(cond_cmp[i] == '='){
            if(temp == cond_val[i]){
                continue;
            }
            else{
                return false;
            }
        }
        else if(cond_cmp[i] == '<'){
            if(cond_pos[i] == table_ptr_->prim_key_){
                if(key_range[1] == NULL){
                    continue;
                }
                else if(temp < *key_range[1]){
                    continue;
                }
                else{
                    if(stop_flag == 1){
                        stop_flag = 2;
                    }
                    return false;
                }
            }
            else if(temp < cond_val[i]){
                continue;
            }
            else{
                return false;
            }
        }
        else if(cond_cmp[i] == '>'){
            if(cond_pos[i] == table_ptr_->prim_key_){
                if(key_range[0] == NULL){
                    continue;
                }
                else if(temp > *key_range[0]){
                    continue;
                }
                else{
                    if(stop_flag == 1){
                        stop_flag = 2;
                    }
                    return false;
                }
            }
            else if(temp > cond_val[i]){
                continue;
            }
            else{
                return false;
            }
        }
    }
    return true;
}