#include <Data.h>
#include <Implement.h>

//构造函数
Page::Page(Table* table){
    is_empty_ = false;
    is_full_ = false;
    table_ptr_ = table;
    page_index_ = *new Index(table->getKeyType());
    cursor_pos_ = 0;
    rows_ = NULL;
    rows_ = new Row*[table_ptr_->max_rows_per_page_];
}  

//打印整页
void Page::print_page(){
    for(__uint16_t i = 0;i<cursor_pos_;i++){
        cout<<"| ["<<i<<"]\table_ptr_| "<<rows_[i]->Format();
    }cout<<"| - - - +";
    for(int i = 0;i<table_ptr_->parm_num_;i++){
        cout<<" - - - -";
    }cout<<" [Page Index : "<<page_index_<<"]\table_ptr_"<<endl;
}

