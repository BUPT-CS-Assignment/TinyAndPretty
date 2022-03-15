#include <Data.h>
#include <Implement.h>

//数据表创建
Table::Table(int ID, string name = "New Table"){
    __ID = ID;
    __Cursor = 0;
    __ElementNumber = 0;
    __Name = name;
    __Pages = new Page*[100];   //最大页数:100
}


bool Table::Init(string statement){
    /*
        数据表初始化

    */
    string* words = Split(statement,',',__ElementNumber);
    __ElementType = new __DataType__[__ElementNumber];
    __ElementID = new string[__ElementNumber];
    for(int i =0;i<__ElementNumber;i++){
        string temp = words[i];
        int index= temp.find(':');
        __ElementID[i] = temp.substr(0,index);
        string elementType = temp.substr(index+1,temp.length()-index);
        if(elementType.compare("int") == 0){
            __ElementType[i] = __Int;
        }else if(elementType.compare("varchar32") == 0){
            __ElementType[i] = __VarChar32;
        }else if(elementType.compare("varchar255") == 0){
            __ElementType[i] = __VarChar255;
        }else{
            cout<<"Unknown Element Type."<<endl;
            Remove();
            return false;
        }
    }
    return true;
}


bool Table::Insert(string statement){
    /*
        在数据表中插入数据行
    */
    Row* new_row = new Row(*this);   //创建行对象
    //填充数据
    if(! new_row->Padding(*this,statement)){
        return false;
    }   
    if(__Cursor == 0 || __Pages[__Cursor - 1]->IsFull()){
        __Pages[__Cursor] = new Page(__Cursor);
        __Cursor++;
    }
    __Pages[__Cursor-1]->Insert(new_row);
    return true;
}


void Table::Print(){
    cout<<"\t";
    for(int i = 0;i<__ElementNumber;i++){
        cout<<"["<<__ElementID[i]<<"]\t";
    }cout<<endl;
    for(int i =0;i<__Cursor;i++){
        __Pages[i]->Print(*this);
    }
}

void Table::AddPage(Page* Node){
    __Pages[__Cursor] = Node;
    ++ __Cursor;
}

bool Table::DeleteRow(int page_number,int row_number){
    if(page_number > __Cursor -1){
        cout<<"Folio --"<<page_number<<"-- Out Of Bounds '"<<__Cursor-1<<"'."<<endl;
        return false;
    }
    if(! __Pages[page_number]->Delete(row_number)){
        return false;
    }
    return true;
}

bool Table::DeletePage(int page_number){
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

bool Table::Remove(){
    for(int i = 0;i<__Cursor;i++){
        delete[] __Pages[i];
    }
    delete[] __ElementType;
    delete[] __ElementID;
    return true;
}

__DataType__* Table::getElementType(){
    return __ElementType;
}

int Table::getElementNumber(){
    return __ElementNumber;
}

string Table::getName(){
    return __Name;
}

Page* Table::getPage(int page_number){
    if(page_number >= __Cursor) return NULL;
    return __Pages[page_number];
}
