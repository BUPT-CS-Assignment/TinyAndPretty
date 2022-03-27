#include<Storage.h>
Memorizer::Memorizer(){
    //cout<<"<S> I/O"<<endl;
    //cout<<"RAM INITIALIZE"<<endl;
}
/**
 * .ldf //表架构文件
 * .ldd //数据文件
 * .ldi //索引文件
 */

void Memorizer::TableStore(Table* table){
    if(table == NULL) return;
    //检测空文件/创建文件
    string filePath = kHomeDir + table->table_name_ + kFramSuffix;
    FILE* fp = fopen(filePath.c_str(),"w");
    fseek(fp,0,SEEK_SET);
    fwrite(&table->table_id_, 4 , 1 ,fp);
    fwrite(table->table_name_, 32, 1,fp);
    fwrite(&table->total_pages_, 2,1,fp);
    fwrite(table->empty_pages_,2,1+MAX_EMPTY_PAGE,fp);
    fwrite(&table->parm_num_,2,1,fp);
    fwrite(table->parm_types_,2,table->parm_num_,fp);
    fwrite(table->parm_names_,32,table->parm_num_,fp);
    fwrite(&table->prim_key_,2,1,fp);
    fwrite(&table->row_take_up_,2,1,fp);
    fwrite(&table->max_rows_per_page_,2,1,fp);
    fclose(fp);
}

Table* Memorizer::TableLoad(string table_name){ 
    string filePath = kHomeDir + table_name + kFramSuffix;
    string dataPath = kHomeDir + table_name + kDataSuffix;
    try{
        FILE* fp;
        fp = fopen(filePath.c_str(),"r");
        fseek(fp,0,SEEK_SET);
        Table* table = new Table(0," ");
        fread(&table->table_id_,4,1,fp);
        fread(table->table_name_,32,1,fp);
        fread(&table->total_pages_, 2,1,fp);
        fread(table->empty_pages_,2,1 + MAX_EMPTY_PAGE,fp);
        fread(&table->parm_num_,2,1,fp);
        table->parm_types_ = new DATA_TYPE[table->parm_num_];
        table->parm_names_ = new char[table->parm_num_][32]{0};
        fread(table->parm_types_,2,table->parm_num_,fp);
        fread(table->parm_names_,32,table->parm_num_,fp);
        fread(&table->prim_key_,2,1,fp);
        fread(&table->row_take_up_,2,1,fp);
        fread(&table->max_rows_per_page_,2,1,fp);
        fclose(fp);
        //主键索引树重构
        if(table->total_pages_ == 0) return table;
        fp = fopen(dataPath.c_str(),"r");
        bool isEmpty = true;
        for(__uint16_t i = 0; i < table->total_pages_; i++){
            fseek(fp,PAGE_SIZE * i , SEEK_SET);
            fread(&isEmpty,1,1,fp);
            if(isEmpty) continue;
            Index* index = new Index(table->getKeyType());
            fread(&index->index_,index->getSize(),1,fp);
            table->pages_tree_->InsertData(index,new __uint16_t(i));
        }
        fclose(fp);
        return table;
    }catch(exception &e){
        return NULL;
    }
}

void Memorizer::TableUpdate(Table* table){
    if(table == NULL) return;
    //检测空文件/创建文件
    string filePath = kHomeDir + table->table_name_ + kFramSuffix;
    FILE* fp = fopen(filePath.c_str(),"r+");
    fseek(fp,36,SEEK_SET);
    fwrite(&table->total_pages_,2,1,fp);
    fwrite(table->empty_pages_,2,1+MAX_EMPTY_PAGE,fp);
    fclose(fp);
}

Page* Memorizer::PageLoad(__uint16_t offset, Table* table){
    string filePath = kHomeDir + table->table_name_ + kDataSuffix;
    try{
        FILE* fp = fopen(filePath.c_str(),"r");
        fseek(fp,offset * PAGE_SIZE,SEEK_SET);
        bool isEmpty;
        fread(&isEmpty,1,1,fp);
        if(isEmpty) return NULL;
        Page* page = new Page(table);
        page->is_empty_ = false;
        fread(&page->page_index_.index_,page->page_index_.getSize(),1,fp);
        fread(&page->is_full_,1,1,fp);
        fread(&page->cursor_pos_,2,1,fp);
        for(__uint16_t i = 0; i < page->cursor_pos_; i++){
            Row* row = new Row(table);
            for(__uint16_t j = 0; j < table->parm_num_; j++){
                fread(row->content_[j],kTypeSize[table->parm_types_[j]],1,fp);
                row->index_update();
            }
            page->rows_[i] = row;
        }
        fclose(fp);
        return page;
    }catch(exception &e){
        return NULL;
    }
}

void Memorizer::PageFlush(__uint16_t offset, Table* table){
    string filePath = kHomeDir + table->table_name_ + kDataSuffix;
    try{
        FILE* fp;
        fp = fopen(filePath.c_str(),"ab+");
        fclose(fp);
        fp = fopen(filePath.c_str(),"r+");
        fseek(fp,offset * PAGE_SIZE,SEEK_SET);
        char ch[PAGE_SIZE];
        memset(ch,0,sizeof(ch));
        fwrite(ch,sizeof(ch),1,fp);
        fclose(fp);
    }catch(exception &e){
        return;
    }
}


void Memorizer::PageStore(__uint16_t offset, Page* page){
    if(page == NULL) return;
    string filePath = kHomeDir + page->table_ptr_->table_name_ + kDataSuffix;
    try{
        FILE* fp;
        fp = fopen(filePath.c_str(),"a");
        fclose(fp);
        fp = fopen(filePath.c_str(),"r+");
        fseek(fp,offset * PAGE_SIZE,SEEK_SET);
        fwrite(&page->is_empty_,1,1,fp);
        fwrite(&page->page_index_.index_,page->page_index_.getSize(),1,fp);
        fwrite(&page->is_full_,1,1,fp);
        fwrite(&page->cursor_pos_,2,1,fp);
        for(__uint16_t i = 0; i < page->cursor_pos_; i++){
            Row* row = page->rows_[i];
            for(__uint16_t j = 0; j < page->table_ptr_->parm_num_; j++){
                fwrite(row->content_[j],kTypeSize[page->table_ptr_->parm_types_[j]],1,fp);
            }
        }
        fclose(fp);
    }catch(exception &e){
        return;
    }
}





void Memorizer::IndexLoad(){

}