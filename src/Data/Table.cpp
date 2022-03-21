#include<Data.h>
#include<Implement.h>

//数据表创建
Table::Table(int ID, string name){
    __Table_ID = ID;
    __Page_Num = 0;
    __Data_Num = 0;
    __Page_ID = 0;
    __Name = name;
    __Pages = new BalanceTree<Page,Index>(ID);

}

/*
    
    CREATE table_name(ID:int,
                      Name:varchar32,
                      School:varchar255,
                      KEY(xxx)
                      );
    *省略KEY()则默认首元素为主键

*/
bool Table::init(string statement){
    /*
        数据表初始化
    */
    if(statement.length() == 0){
        cout<<"Parameter Empty."<<endl;
        return false;
    }
    string* words = Split(statement,',',__Data_Num);
    __Data_Type = new __DataType__[__Data_Num];
    __Data_ID = new string[__Data_Num];
    bool key_assigned = false;
    string assign_key = "";
    int data_number = __Data_Num, pos = 0;
    for(int i =0;i<data_number;i++){
        string temp = words[i];
        //匹配语句
        if(regex_match(temp,regex("\\w+[:]\\w+"))){
            int index = temp.find(":");
            __Data_ID[pos] = Trim(temp.substr(0,index));
            string elementType = Trim(temp.substr(index+1,temp.length()-index));
            if(elementType.compare("INT") == 0){
                __Data_Type[pos++] = __INT;
            }else if(elementType.compare("INT64") == 0){
                __Data_Type[pos++] = __INT64;
            }else if(elementType.compare("REAL") == 0){
                __Data_Type[pos++] = __REAL;
            }else if(elementType.compare("TEXT") == 0){
                __Data_Type[pos++] = __TEXT;
            }else if(elementType.compare("LONGTEXT") == 0){
                __Data_Type[pos++] = __LONGTEXT;
            }else{
                cout<<"Unknown Element Type."<<endl;
                remove_table();
                return false;
            }
        }else if(regex_match(temp,regex("^KEY[(]\\w+[)]$"))){
            __Data_Num--;
            //匹配索引项
            if(key_assigned){
                int index_f = temp.find("(");
                int index_t = temp.find(")");
                if(assign_key != temp.substr(index_f + 1,index_t - index_f -1)){
                    cout<<"PRIMARY_KEY Already Assigned : "<<assign_key<<endl;
                    return false;
                }
            }else{
                key_assigned = true;
                int index_f = temp.find("(");
                int index_t = temp.find(")");
                assign_key = temp.substr(index_f + 1,index_t - index_f -1);
                cout<<"PRIMARY_KEY for Table '"<<__Name<<"' : "<<assign_key<<endl;
            }
        }else{
            cout<<"Malform Parameter."<<endl;
            return false;
        }
    }
    //指定索引
    if(key_assigned){
        for(int i = 0;i<__Data_Num;i++){
            if(__Data_ID[i].compare(assign_key) == 0){
                __Index_ID = assign_key;
                __Index_Type = __Data_Type[i];
                return true;
            }
        }
        cout<<"Assigned KEY '"<<assign_key<<"' Not Found."<<endl;
        return false;
    }
    //未指定索引,默认第一元素为索引
    __Index_ID = __Data_ID[0];
    __Index_Type = __Data_Type[0];
    return true;
}


bool Table::insert(string statement){
    /*
        在数据表中插入数据行
    */
    Row* new_row = new Row(this);   //创建行对象
    //填充数据
    if(! new_row->padding(statement)){
        return false;
    } 
    //查找插入数据页指针
    Page* data = __Pages->locate_insert_data(&(new_row->getIndex()));
    ///////数据页可插入
    if(data!= NULL && !data->isFull()){
        if(!data->insert(new_row)){
            cout<<"Primary Key '"<<new_row->getIndex()<<"' Already Exsits."<<endl;
            return false;
        }
        return true;
    }
    //////新建页后插入
    Page* new_page = new Page(__Page_ID ++ ,this);
    //////////////////
    if(data != NULL && data->isFull()){
        for(int i = MAX_ROWS_SINGLE_PAGE/2;i < MAX_ROWS_SINGLE_PAGE; i++){
            new_page->insert(data->__Rows[i]);
            data->__Rows[i] = NULL;
            data->__Cursor--;
        }
        new_page->insert(new_row);
        new_page->__Index = new_page->__Rows[0]->getIndex();
        __Pages->insert_data(&(new_page->__Index),new_page);
        return true;
    }
    new_page->insert(new_row);
    new_page->__Index = new_row->getIndex();
    __Pages->insert_data(&(new_row->getIndex()),new_page);
    return true;
}



bool Table::delete_row(Index& index){
    Page* data = __Pages->locate_insert_data(&index);
    if(data == NULL){
        cout<<"<Data Not Found>"<<endl;
        return false;
    }
    if(!data->delete_row(index)) return false;
    Index temp = *(new Index(data->__Index));
    if(data->__Cursor == 0){
        data->remove_page();
        __Pages->delete_data(&temp);
    } 
    return true;
}

bool Table::delete_page(Index& index){
    Page* data = __Pages->locate_data(&index);
    if(data == NULL){
        cout<<"<Page Not Found>"<<endl;
        return false;
    }
    data->remove_page();
    __Pages->delete_data(&index);
    return true;
}

bool Table::remove_table(){/*
    for(int i = 0;i<__Cursor;i++){
        __Pages[i]->remove_page();
        delete[] __Pages[i];
    }
    */
    delete[] __Data_Type;
    delete[] __Data_ID;
    return true;
}

void Table::print_table(){
    /*
    cout<<"+-----------------------------------------"<<endl;
    cout<<"|             "<<__Name<<"               "<<endl;
    */
    cout<<"+-------+---------------------------------"<<endl;
    cout<<"| [R]   |";
    for(int i = 0;i<__Data_Num;i++){
        cout<<" "<<__Data_ID[i];
        if(__Data_Type[i]==__Index_Type) cout<<"[P]";
        cout<<"\t";
    }cout<<endl;
    cout<<"+-------+---------------------------------"<<endl;
    Node<Page,Index>* head = __Pages->getHead();
    if(head == NULL) return;
    while(head != NULL){
        for(int i = 0; i >= 0 ; i++){
            Page* page = head->getData(i);
            if(page == NULL) break;
            page->print_page();
        }
        head = head->getNext();
    }
    cout<<"+-------+---------------------------------"<<endl;
}

void Table::print_structure(){
    /*
    cout<<"+-----------------------"<<endl;
    cout<<"|   "<<__Name<<"   "<<endl;
    */
    cout<<"+-----------+-----------"<<endl;
    cout<<"|   Filed   |   Type   "<<endl;
    cout<<"+-----------+-----------"<<endl;
    for(int i = 0; i < __Data_Num; i++){
        cout<<"| ";
        cout<<setw(10)<<left<<__Data_ID[i]+(__Data_Type[i]==__Index_Type?"[P]":"");
        cout<<"| "<<__Type__[__Data_Type[i]]<<endl;
    }
    cout<<"+-----------+-----------\n"<<endl;
}


__DataType__* Table::getDataType(){
    return __Data_Type;
}

__DataType__ Table::getIndexType(){
    return __Index_Type;
}

string Table::getName(){
    return __Name;
}

