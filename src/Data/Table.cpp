#include<Data/Table.h>
#include<Data/Page.h>
#include<Data/Row.h>
#include<Implement.h>

//数据表创建
Table::Table(int ID, string name){
    __Table_ID = ID;
    __Cursor = 0;
    __Data_Num = 0;
    __Name = name;
    __Pages = new Page*[MAX_PAGES_SINGLE_TABLE];   //最大页数:100
}


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
    for(int i =0;i<__Data_Num;i++){
        string temp = words[i];
        int index= temp.find(':');
        if(index <= 0){
            cout<<"Malform Parameter."<<endl;
            return false;
        }
        __Data_ID[i] = Trim(temp.substr(0,index));
        string elementType = Trim(temp.substr(index+1,temp.length()-index));
        if(elementType.compare("short") == 0){
            __Data_Type[i] = __Short;
        }else if(elementType.compare("int") == 0){
            __Data_Type[i] = __Int;
        }else if(elementType.compare("uint") == 0){
            __Data_Type[i] = __UInt;
        }else if(elementType.compare("char") == 0){
            __Data_Type[i] = __Char;
        }else if(elementType.compare("varchar11") == 0){
            __Data_Type[i] = __VarChar11;
        }else if(elementType.compare("varchar32") == 0){
            __Data_Type[i] = __VarChar32;
        }else if(elementType.compare("varchar255") == 0){
            __Data_Type[i] = __VarChar255;
        }else{
            cout<<"Unknown Element Type."<<endl;
            remove_table();
            return false;
        }
    }
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
    if(__Cursor == 0 || __Pages[__Cursor - 1]->isFull()){
        __Pages[__Cursor] = new Page(__Cursor,this);
        __Cursor++;
    }
    __Pages[__Cursor-1]->insert(new_row);
    return true;
}


void Table::print_table(){
    cout<<"\t";
    for(int i = 0;i<__Data_Num;i++){
        cout<<"["<<__Data_ID[i]<<"]\t";
    }cout<<endl;
    for(int i =0;i<__Cursor;i++){
        __Pages[i]->print_page();
    }
}

void Table::add_page(Page* Node){
    __Pages[__Cursor] = Node;
    ++ __Cursor;
}

bool Table::delete_row(int page_number,int row_number){
    if(page_number > __Cursor -1){
        cout<<"Folio --"<<page_number<<"-- Out Of Bounds '"<<__Cursor-1<<"'."<<endl;
        return false;
    }
    if(! __Pages[page_number]->delete_row(row_number)){
        return false;
    }
    return true;
}

bool Table::delete_page(int page_number){
    if(page_number > __Cursor -1){
        cout<<"Folio --"<<page_number<<"-- Out Of Bounds '"<<__Cursor-1<<"'."<<endl;
        return false;
    }
    delete[] __Pages[page_number];
    for(int i = page_number+1;i<__Cursor;i++){
        __Pages[i-1] = __Pages[i];
    }
    __Cursor--;
    return true;
}

bool Table::remove_table(){
    for(int i = 0;i<__Cursor;i++){
        __Pages[i]->remove_page();
        delete[] __Pages[i];
    }
    delete[] __Data_Type;
    delete[] __Data_ID;
    return true;
}

__DataType__* Table::getDataType(){
    return __Data_Type;
}


string Table::getName(){
    return __Name;
}

Table::Page* Table::getPage(int page_number){
    if(page_number >= __Cursor) return NULL;
    return __Pages[page_number];
}
