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
        if((fp = fopen(filePath.c_str(),"r")) == NULL){
            cout<<"<E> FILE NOT EXIST"<<endl;
            return NULL;
        }
        fseek(fp,0,SEEK_SET);
        Table* table = new Table(0," ");
        size_t res;
        res = fread(&table->table_id_,4,1,fp);
        res = fread(table->table_name_,32,1,fp);
        res = fread(&table->parm_num_,2,1,fp);
        table->parm_types_ = new DATA_TYPE[table->parm_num_];
        table->parm_names_ = new char[table->parm_num_][32]{{0}};
        res = fread(table->parm_types_,2,table->parm_num_,fp);
        res = fread(table->parm_names_,32,table->parm_num_,fp);
        res = fread(&table->prim_key_,2,1,fp);
        res = fread(&table->row_take_up_,2,1,fp);
        res = fread(&table->max_rows_per_page_,2,1,fp);
        if(res < 0) return NULL;
        fclose(fp);
        //主键索引树重构
        //if(table->max_offset == 0) return table;
        if((fp = fopen(dataPath.c_str(),"r")) == NULL){
            cout<<"<W> DATA SOURCE FILE NOT FOUND : "<<table_name<<endl;
            return table;
        }
        res = fread(&table->max_offset,2,1,fp);
        bool notEmpty = true;
        for(__uint16_t i = 0; i < table->max_offset; i++){
            fseek(fp,DATA_OFFSET + PAGE_SIZE * i , SEEK_SET);
            res = fread(&notEmpty,1,1,fp);
            if(!notEmpty){
                table->add_empty_page(i);
                continue;
            } 
            Index* index = new Index(table->getKeyType());
            res = fread(&index->index_,index->getSize(),1,fp);
            table->pages_tree_->InsertData(index,new __uint16_t(i));
        }
        fclose(fp);
        return table;
    }catch(exception &e){
        return NULL;
    }
}


Page* Memorizer::PageLoad(__uint16_t offset, Table* table){
    string filePath = kHomeDir + table->table_name_ + kDataSuffix;
    try{
        FILE* fp;
        if((fp = fopen(filePath.c_str(),"r")) == NULL){
            cout<<"<E> DATA SOURCE FILE MISSING"<<endl;
            return NULL;
        }
        size_t res;
        fseek(fp,DATA_OFFSET + offset * PAGE_SIZE,SEEK_SET);
        bool notEmpty;
        res = fread(&notEmpty,1,1,fp);
        if(!notEmpty) return NULL;
        Page* page = new Page(table);
        page->not_empty_ = true;
        res = fread(&page->page_index_.index_,page->page_index_.getSize(),1,fp);
        res = fread(&page->is_full_,1,1,fp);
        res = fread(&page->cursor_pos_,2,1,fp);
        for(__uint16_t i = 0; i < page->cursor_pos_; i++){
            Row* row = new Row(table);
            for(__uint16_t j = 0; j < table->parm_num_; j++){
                res = fread(row->content_[j],kTypeSize[table->parm_types_[j]],1,fp);
                row->index_update();
            }
            page->rows_[i] = row;
        }
        if(res < 0) return NULL;
        fclose(fp);
        return page;
    }catch(exception &e){
        cout<<"<E> DATA SOURCE FILE DAMAGED"<<endl;
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
        fwrite(&table->max_offset,2,1,fp);
        fseek(fp,DATA_OFFSET + offset * PAGE_SIZE,SEEK_SET);
        //fseek(fp,DATA_OFFSET + offset * PAGE_SIZE,SEEK_SET);
        char ch[PAGE_SIZE];
        memset(ch,0,sizeof(ch));
        fwrite(ch,sizeof(ch),1,fp);
        fclose(fp);
    }catch(exception &e){
        cout<<"<E> DATA SOURCE FILE DAMAGED"<<endl;
        return;
    }
}


void Memorizer::PageStore(__uint16_t offset, Page* page){
    if(page == NULL) return;
    string filePath = kHomeDir + page->table_ptr_->table_name_ + kDataSuffix;
    try{
        FILE* fp;
        fp = fopen(filePath.c_str(),"ab+");
        fclose(fp);
        fp = fopen(filePath.c_str(),"r+");
        fwrite(&page->table_ptr_->max_offset,2,1,fp);
        fseek(fp,DATA_OFFSET + offset * PAGE_SIZE,SEEK_SET);
        fwrite(&page->not_empty_,1,1,fp);
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
        cout<<"<E> DATA SOURCE FILE DAMAGED"<<endl;
        return;
    }
}





void Memorizer::IndexLoad(){

}