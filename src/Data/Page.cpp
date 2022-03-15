#include <Data.h>
#include <Implement.h>

//构造函数
Page::Page(int folio){
    __Folio = folio;
    __MaxRows = MAX_ROWS_SINGLE_PAGE;
    __Rows = new Row*[__MaxRows];
    __Index = 0;
    __Cursor = 0;
}   

//插入行
void Page::Insert(Row* Node){
    __Rows[__Cursor] = Node;
    ++ __Cursor;
}

//删除指定行
bool Page::Delete(int row_number){
    if(row_number > __Cursor-1){
        cout<<"Row Number --"<<row_number<<"-- Out Of Bounds '"<<__Cursor-1<<"' At Page '"<<__Folio<<"'."<<endl;
        return false;
    }
    delete[] __Rows[row_number];
    for(int i = row_number+1;i<__Cursor;i++){
        __Rows[i-1] = __Rows[i];
    }
    __Cursor--;
    return true;
}

//判断页面是否已满
bool Page::IsFull(){
    return __Cursor >= __MaxRows;
}

//返回光标位置
int Page::getCursor(){
    return __Cursor;
}

//删除整页
bool Page::Remove(){
    for(int i = 0;i<__Cursor;i++){
        delete[] __Rows[i];
    }
    return true;
}

//打印整页
void Page::Print(Table table){
    for(int i = 0;i<__Cursor;i++){
        cout<<"["<<i<<"]\t"<<__Rows[i]->Format(table);
    }
    for(int i = 0;i<table.getElementNumber();i++){
        cout<<"\t";
    }cout<<"[Page : "<<__Folio<<"]\t\n"<<endl;
}