#include<Data/Table.h>
#include<Data/Page.h>
#include<Data/Row.h>
#include <Implement.h>

//构造函数
Table::Page::Page(int page_num,Table* t){
    this->t = t;
    __Page_Num = page_num;
    __Rows = new Row*[MAX_ROWS_SINGLE_PAGE];
    __Cursor = 0;
}   

//插入行
void Table::Page::insert(Row* Node){
    __Rows[__Cursor] = Node;
    ++ __Cursor;
}

//删除指定行
bool Table::Page::delete_row(int row_number){
    if(row_number > __Cursor-1){
        cout<<"Row Number --"<<row_number<<"-- Out Of Bounds '"<<__Cursor-1<<"' At Page '"<<__Page_Num<<"'."<<endl;
        return false;
    }
    __Rows[row_number]->erase();
    //delete[] __Rows[row_number];
    for(int i = row_number;i<__Cursor -1;i++){
        __Rows[i] = __Rows[i+1];
    }
    __Cursor--;
    return true;
}

//判断页面是否已满
bool Table::Page::isFull(){
    return __Cursor >= MAX_ROWS_SINGLE_PAGE;
}

//返回光标位置
int Table::Page::getCursor(){
    return __Cursor;
}

//删除整页
bool Table::Page::remove_page(){
    for(int i = 0;i<__Cursor;i++){
        __Rows[i]->erase();
        //delete[] __Rows[i];
    }
    return true;
}

//打印整页
void Table::Page::print_page(){
    for(int i = 0;i<__Cursor;i++){
        cout<<"["<<i<<"]\t"<<__Rows[i]->format();
    }
    for(int i = 0;i<t->__Data_Num;i++){
        cout<<"\t";
    }cout<<"[Page : "<<__Page_Num<<"]\t\n"<<endl;
}