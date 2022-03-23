#include<Data.h>
#include<Implement.h>

//数据表创建
Table::Table(int ID, string name){
    __TableID = ID;
    __TableName = name;
    __TableHeadSize = 0;
    __ParmNum = 0;
    __MaxRowPerPage = 0;
    __RowTakeup = 0;
    __Key = 0;
    __RowTakeup = 0;
    __MaxRowPerPage = 0;
    __TotalPage = 0;
    __EmptyPage[0] = 0;
    __Pages = new BalanceTree<__uint16_t,Index>(ID);

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
    
    int number;
    string* words = Split(statement,',',number);
    __ParmNum = number;
    __ParmType = new __DataType__[__ParmNum];
    __ParmName = new string[__ParmNum];
    bool key_assigned = false;
    string assign_key = "";
    int data_number = __ParmNum, pos = 0;
    for(int i =0;i<data_number;i++){
        string temp = words[i];
        //匹配语句
        if(regex_match(temp,regex("\\w+[:]\\w+"))){
            int index = temp.find(":");
            __ParmName[pos] = Trim(temp.substr(0,index));
            string elementType = Trim(temp.substr(index+1,temp.length()-index));
            if(elementType.compare("INT") == 0){
                __ParmType[pos++] = __INT;
                __RowTakeup += 4;
            }else if(elementType.compare("INT64") == 0){
                __ParmType[pos++] = __INT64;
                __RowTakeup += 8;
            }else if(elementType.compare("REAL") == 0){
                __ParmType[pos++] = __REAL;
                __RowTakeup += 8;
            }else if(elementType.compare("TEXT") == 0){
                __ParmType[pos++] = __TEXT;
                __RowTakeup += 32;
            }else if(elementType.compare("LONGTEXT") == 0){
                __ParmType[pos++] = __LONGTEXT;
                __RowTakeup += 255;
            }else{
                cout<<"Unknown Element Type."<<endl;
                remove_table();
                return false;
            }
        }else if(regex_match(temp,regex("^KEY[(]\\w+[)]$"))){
            __ParmNum--;
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
                cout<<"PRIMARY_KEY for Table '"<<__TableName<<"' : "<<assign_key<<endl;
            }
        }else{
            cout<<"Malform Parameter."<<endl;
            return false;
        }
    }
    __MaxRowPerPage = (BYTES_PER_PAGE - PAGE_HEAD_SIZE)/__RowTakeup;
    __TableHeadSize = 50 + 2*MAX_EMPTY_PAGE + __ParmNum*(34);
    //指定索引
    __Key = 0;
    if(key_assigned){
        for(int i = 0;i<__ParmNum;i++){
            if(__ParmName[i].compare(assign_key) == 0){
                __Key = i;
                if(__ParmType[__Key] == __LONGTEXT){
                    cout<<"Type 'LONGTEXT' Cannot Be Used As PRIMAEY KEY."<<endl;
                    return false;
                }
                Memorizer M(this);
                M.storage();
                return true;
            }
        }
        cout<<"Assigned KEY '"<<assign_key<<"' Not Found."<<endl;
        return false;
    }
    if(__ParmType[__Key] == __LONGTEXT){
        cout<<"Type 'LONGTEXT' Cannot Be Used As PRIMAEY KEY."<<endl;
        return false;
    }
    Memorizer T(this);
    T.storage();
    return true;
}

/////////////////////////////////////////////////////////////////////

bool Table::insert(string statement){
    /*
        在数据表中插入数据行
    */
    
    Row* new_row = new Row(this);   //创建行对象
    //填充数据
    if(! new_row->padding(statement)){
        return false;
    } 
    Memorizer M(this,NULL);
    //查找插入数据页指针
    __uint16_t* page_offset = __Pages->locate_insert_data(&(new_row->getIndex()));
    ////////////////////////////////////////////////////////////////////
    if(page_offset == NULL){
        /////计算偏移量/////////////////////////////////////////////////////
        __uint16_t* new_page_offset = new __uint16_t(get_empty_page_offset());
        ////////////////////////////////////////////////////////////////////
        Page* new_page = new Page(*new_page_offset,this);
        new_page->insert(new_row);
        new_page->__PageIndex = *new Index(new_row->getIndex());
        __Pages->insert_data(&(new_row->getIndex()),new_page_offset);
        //new_page->print_page();
        M.change_page(new_page);
        M.page_flush(*new_page_offset);
        M.page_write_back(*new_page_offset);
        __TotalPage ++ ;
        M.update();
        return true;
    }
    /////读入内存//////////////////////////////////////////////////////
    Page* page = new Page(this);
    M.change_page(page);
    M.page_read_in(*page_offset);
    if(!page->__IsFull){
        if(!page->insert(new_row)){
            cout<<"Primary Key '"<<new_row->getIndex()<<"' Already Exsits."<<endl;
            return false;
        }
        M.page_write_back(*page_offset);
    }else{
    //////新建页后插入/////////////////////////////////////////////////
        Page* new_page = new Page(this);
        __uint16_t* new_page_offset = new __uint16_t(get_empty_page_offset());
        cout<<"new page offset : "<<endl;
        for(int i = __MaxRowPerPage/2;i <__MaxRowPerPage; i++){
            new_page->insert(page->__Rows[i]);
            page->__Rows[i]->erase();
            page->__CursorOffset--;
        }
        new_page->insert(new_row);
        new_page->__PageIndex = *new Index(new_page->__Rows[0]->getIndex());
        __Pages->insert_data(&(new_page->__PageIndex),new_page_offset);
        M.page_write_back(*page_offset);
        /////////////////////////////////////////////////////////////
        M.change_page(new_page);
        M.page_flush(*new_page_offset);
        M.page_write_back(*new_page_offset);
        __TotalPage ++ ;
        page->remove_page();
        new_page->remove_page();
    }
    M.update();
    return true;
}


bool Table::delete_row(Index& index){
    
    //查找插入数据页指针
    Page* page = new Page(this);
    Memorizer M(this,page);
    __uint16_t* page_offset = __Pages->locate_insert_data(&index);
    if(page_offset == NULL){
        cout<<"<Data Not Found>"<<endl;
        return false;
    }
    M.page_read_in(*page_offset);
    if(!page->delete_row(index)) return false;
    Index temp = *(new Index(page->__PageIndex));
    if(page->__CursorOffset == 0){
        if(page->__PageOffset != __TotalPage){
            add_empty_page(page->__PageOffset);
        }
        __Pages->delete_data(&temp);
    }
    M.page_write_back(*page_offset);
    page->remove_page();
    M.update();
    return true;
}

bool Table::delete_page(Index& index){
    //查找插入数据页指针
    __uint16_t* page_offset = __Pages->locate_data(&index);
    if(page_offset == NULL){
        cout<<"<Data Not Found>"<<endl;
        return false;
    }
    Page* page = new Page(this);
    Memorizer M(this,page);
    M.page_read_in(*page_offset);
    Index temp = *(new Index(page->__PageIndex));
    __Pages->delete_data(&temp);
    if(page->__PageOffset != __TotalPage){
        add_empty_page(page->__PageOffset);
    }
    M.page_flush(*page_offset);
    page->remove_page();
    M.update();
    return true;
}

bool Table::remove_table(){/*
    for(int i = 0;i<__Cursor;i++){
        __Pages[i]->remove_page();
        delete[] __Pages[i];
    }
    */
    delete[] __ParmType;
    delete[] __ParmName;
    
    return true;
}

void Table::print_table(){
    /*
    cout<<"+-----------------------------------------"<<endl;
    cout<<"|             "<<__Name<<"               "<<endl;
    */
    cout<<"+-------+---------------------------------"<<endl;
    cout<<"| [R]   |";
    for(int i = 0;i<__ParmNum;i++){
        cout<<" "<<__ParmName[i];
        if(i==__Key) cout<<"[P]";
        cout<<"\t";
    }cout<<endl;
    cout<<"+-------+---------------------------------"<<endl;
    Node<__uint16_t,Index>* head = __Pages->getHead();
    if(head == NULL) return;
    Memorizer M(this,NULL);
    while(head != NULL){
        for(int i = 0; i >= 0 ; i++){
            __uint16_t* page_offset =  head->getData(i);
            if(page_offset == NULL) break;
            Page* page = new Page(this);
            M.change_page(page);
            M.page_read_in(*page_offset);
            page->print_page();
            page->remove_page();
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
    for(int i = 0; i < __ParmNum; i++){
        cout<<"| ";
        cout<<setw(10)<<left<<__ParmName[i]+(i==__Key?"[P]":"");
        cout<<"| "<<__Type__[__ParmType[i]]<<endl;
    }
    cout<<"+-----------+-----------\n"<<endl;
}

string Table::getName(){
    return __TableName;
}

__DataType__ Table::getKeyType(){
    return __ParmType[__Key];
}

bool Table::check_empty(int page_num){
    int total = __EmptyPage[0];
    if(total <= 0) return false;
    for(int i = 1;i<=total;i++){
        if(page_num == __EmptyPage[i]) return true;
    }
    return false;
}

__uint16_t Table::get_empty_page_offset(){
    int total = __EmptyPage[0];
    if(total == 0) return __TotalPage;
    total -- ;
    return __EmptyPage[total+1]-1;
}

void Table::add_empty_page(__uint16_t page_num){
    __EmptyPage[0]++;
    __EmptyPage[__EmptyPage[0]] = page_num;
}