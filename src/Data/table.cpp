#include<Data.h>
#include<Implement.h>

//数据表创建
Table::Table(int id, string name){
    table_id_ = id;
    strcpy(table_name_, name.c_str());
    max_offset = 0;
    parm_num_ = 0;
    prim_key_ = 0;
    row_take_up_ = 0;
    max_rows_per_page_ = 0;
    parm_names_ = NULL;
    pages_tree_ = new BalanceTree<__uint16_t, Index>(id);
    index_tree_ = NULL;
}


bool Table::Init(string parameters){
    /*
        数据表初始化, 判空
    */
    if(parameters.length() == 0){
        cout << "<E> PARAMETER EMPTY" << endl;
        return false;
    }
    //参数分离
    int number;
    string *params = Split(parameters, ',', number);
    parm_num_ = number;
    //分配空间
    index_tree_ = new BalanceTree<Index, Index>*[parm_num_];
    parm_types_ = new DATA_TYPE[parm_num_];
    parm_names_ = new char[parm_num_][32]{{0}};
    bool key_assigned = 0;
    //参数分析
    for(int i = 0; i < parm_num_; i++){
        index_tree_[i] = NULL;
        string *str = Split(params[i], ' ', number);
        if(number != 2 && number != 3){
            cout << "<E> WRONG PARAMETER FORMAT" << endl;
            return false;
        }
        //数据类型解析
        if(str[1] == "int"){
            parm_types_[i] = __INT;
            row_take_up_ += 4;
        }
        else if(str[1] == "int64"){
            parm_types_[i] = __INT64;
            row_take_up_ += 8;
        }
        else if(str[1] == "real"){
            parm_types_[i] = __REAL;
            row_take_up_ += 8;
        }
        else if(str[1] == "text"){
            parm_types_[i] = __TEXT;
            row_take_up_ += 32;
        }
        else if(str[1] == "longtext"){
            parm_types_[i] = __LONGTEXT;
            row_take_up_ += 255;
        }
        else{
            cout << "<E> UNDEFINED ELEMENT TYPE" << endl;
            return false;
        }
        //存入变量名
        strcpy(parm_names_[i], str[0].c_str());
        //指定主键索引
        if(!key_assigned && number == 3){
            if(parm_types_[i] == __LONGTEXT){
                cout << "<E> PRIM KEY TYPE NOT ALLOWED" << endl;
                return false;
            }
            if(str[2] == "key"){
                key_assigned = true;
                prim_key_ = i;
            }
        }
        //非主键索引创建
        /*
        if(number == 3 && str[2]=="index"){
            if(parm_types_[i] != __LONGTEXT){

                index_tree_[i] = new BalanceTree<Index, Index>(i);
            }else{
                cout<<"<W> INDEX NOT CREATE : TYPE NOT ALLOWED"<<endl;
            }
        }*/
    }
    //计算单页行数
    max_rows_per_page_ = (PAGE_SIZE - PAGE_HEAD_SIZE) / row_take_up_;
    cout << "Take up : " << row_take_up_ << endl;
    cout << "Max : " << max_rows_per_page_ << endl;
    Memorizer RAM;
    RAM.TableStore(this);
    return true;

}


/*
void Table::print_table(){
    cout<<"+-----------------------------------------"<<endl;
    cout<<"|             "<<__Name<<"               "<<endl;
    cout<<"+-------+---------------------------------"<<endl;
    cout<<"| [R]   |";
    for(int i = 0;i<parm_num_;i++){
        cout<<" "<< parm_names_[i];
        if(i==prim_key_) cout<<"[P]";
        cout<<"\table_ptr_";
    }cout<<endl;
    cout<<"+-------+---------------------------------"<<endl;
    Node<__uint16_t,Index>* head =  pages_tree_->getHead();
    if(head == NULL) return;
    Memorizer RAM;
    while(head != NULL){
        for(int i = 0; i >= 0 ; i++){
            __uint16_t* page_offset =  head->getData(i);
            if(page_offset == NULL) break;
            Page* page = RAM.PageLoad(*page_offset,this);
            page->print_page();
            //page->remove_page();
        }
        head = head->getNext();
    }
    cout<<"+-------+---------------------------------"<<endl;
}
*/
void Table::print_structure(){
    /*
    cout<<"+-----------------------"<<endl;
    cout<<"|   "<<__Name<<"   "<<endl;
    */
    cout << "+-----------+-----------" << endl;
    cout << "|   Filed   |   Type   " << endl;
    cout << "+-----------+-----------" << endl;
    for(int i = 0; i < parm_num_; i++){
        cout << "| ";
        string str = parm_names_[i];
        cout << setw(10) << left << str + (i == prim_key_ ? "[P]" : "");
        cout << "| " << kTypeName[parm_types_[i]] << endl;
    }
    cout << "+-----------+-----------\n" << endl;
}

string Table::getName(){
    return table_name_;
}

DATA_TYPE Table::getKeyType(){
    return parm_types_[prim_key_];
}

__uint16_t Table::get_empty_page_offset(){
    /*
    ePage* tail = &empty_pages;
    while(tail!=NULL){
        cout<<tail->offset<<"->";
        tail = tail->next;
    }cout<<endl;
    */
    if(empty_pages.next == NULL){
        max_offset ++;
        return max_offset - 1;
    }
    ///////////////////////////////////////////
    __uint16_t offset = empty_pages.next->offset;
    ePage *temp = empty_pages.next;
    empty_pages.next = empty_pages.next->next;
    delete temp;
    return offset;
}

void Table::add_empty_page(__uint16_t page_num){
    ePage *new_epage = new ePage();
    new_epage->offset = page_num;
    new_epage->next = empty_pages.next;
    empty_pages.next = new_epage;
}

int Table::ParmLocate(string name){
    for(int i = 0; i < parm_num_; i++){
        if(parm_names_[i] == name){
            return i;
        }
    }
    return -1;
}


