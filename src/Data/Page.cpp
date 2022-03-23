#include <Data.h>
#include <Implement.h>

//构造函数
Page::Page(Table* t){
    this->t = t;
    __Rows = new Row*[t->__MaxRowPerPage];
    __CursorOffset = 0;
    __IsFull = 0;
}   

Page::Page(__uint16_t offset, Table* t){
    this->t = t;
    __PageOffset = offset;
    __Rows = new Row*[t->__MaxRowPerPage];
    __CursorOffset = 0;
    __IsFull = 0;
}   

//插入行
bool Page::insert(Row* Node){
    for(__uint16_t i = 0; i < __CursorOffset; i++){
        if(__Rows[i]->getIndex() == Node->getIndex()) return false;
    }
    __Rows[__CursorOffset ++] = Node;
    q_sort(__Rows,0,__CursorOffset-1);
    if(__CursorOffset >= t->__MaxRowPerPage) __IsFull = 1;
    return true;
}

//删除指定行
bool Page::delete_row(Index& index){
    int p = -1;
    for(__uint16_t i = 0; i < __CursorOffset; i++){
        if(__Rows[i]->getIndex() == index){
            p = i;
            break;
        }
    }
    if(p == -1){
        cout<<"<Data Not Found>"<<endl;
        return false;
    } 
    __Rows[p]->erase();
    for(__uint16_t i = p;i<__CursorOffset -1;i++){
        __Rows[i] = __Rows[i+1];
    }
    __CursorOffset--;
    return true;
}


//删除整页
bool Page::remove_page(){
    for(__uint16_t i = 0;i<__CursorOffset;i++){
        __Rows[i]->erase();
    }
    __CursorOffset = 0;
    return true;
}

//打印整页
void Page::print_page(){
    for(__uint16_t i = 0;i<__CursorOffset;i++){
        cout<<"| ["<<i<<"]\t| "<<__Rows[i]->format();
    }cout<<"| - - - +";
    for(int i = 0;i<t->__ParmNum;i++){
        cout<<" - - - -";
    }cout<<" [Page Index : "<<__PageIndex<<"]\t"<<endl;
}

