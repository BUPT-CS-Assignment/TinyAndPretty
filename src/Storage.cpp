#include<Storage.h>

Memorizer::Memorizer(){
    __Table = NULL;
    __Page = NULL;
}

Memorizer::Memorizer(Table* table){
    __Table = table;
    __Page = NULL;
}

Memorizer::Memorizer(Page* page){
    __Table = NULL;
    __Page = page;
}

Memorizer::Memorizer(Table* table,Page* page){
    __Table = table;
    __Page = page;
}

void Memorizer::change_page(Page* page){
    __Page = page;
}

bool Memorizer::load(string filePath){
    FILE* __FilePtr = fopen(filePath.c_str(),"r");
    if(__FilePtr == NULL) return false;
    /////table info read in 
    fseek(__FilePtr,0,SEEK_SET);
    int id;
    char name[32];
    /////table init
    try{
        fread(&id,sizeof(int),1,__FilePtr);
        fread(&name,sizeof(char)*TEXT_LENGTH,1,__FilePtr);
        __Table = new Table(id,name);
        __TABLES__[__CURSOR__++] = __Table;
        fread(&__Table->__TableHeadSize,sizeof(__uint16_t),1,__FilePtr);
        cout<<__Table->__TableHeadSize<<endl;
        //////////////////////////////////////////////////////////////////////
        fread(&__Table->__TotalPage,sizeof(__uint16_t),1,__FilePtr);
        cout<<__Table->__TotalPage<<endl;
        fread(&__Table->__EmptyPage,sizeof(__uint16_t),1+MAX_EMPTY_PAGE,__FilePtr);
        /////////////////////////////////////////////////////////////////////////
        fread(&__Table->__ParmNum,sizeof(__uint16_t),1,__FilePtr);
        __Table->__ParmType = new __DataType__[__Table->__ParmNum];
        __Table->__ParmName = new string[__Table->__ParmNum];
        char temp[TEXT_LENGTH];
        for(int i = 0; i < __Table->__ParmNum; i++){
            fread(&__Table->__ParmType[i],sizeof(__DataType__),1,__FilePtr);
            memset(temp,'\0',sizeof(temp));
            fread(&temp,sizeof(char)*TEXT_LENGTH,1,__FilePtr);
            __Table->__ParmName[i] = *(new string(temp));
        }
        fread(&__Table->__Key,sizeof(__uint16_t),1,__FilePtr);
        fread(&__Table->__RowTakeup,sizeof(__uint16_t),1,__FilePtr);
        fread(&__Table->__MaxRowPerPage,sizeof(__uint16_t),1,__FilePtr);
        __DataType__ type = __Table->__ParmType[__Table->__Key];
        ///// Reinsert Node
        for(__uint16_t i = 0; i < __Table->__TotalPage; i++){
            if(__Table->check_empty(i)) continue;
            fseek(__FilePtr,i*BYTES_PER_PAGE + __Table->__TableHeadSize,SEEK_SET);
            __uint16_t* page_offset = new __uint16_t(0);
            fread(page_offset,sizeof(__uint16_t),1,__FilePtr);
            Index* index = new Index(type);
            fread(&(index->INDEX),sizeof(__INDEX__),1,__FilePtr);
            __Table->__Pages->insert_data(index,page_offset);

        }
        fclose(__FilePtr);

    }catch(exception &e){
        cout<<"File Malfomation, Table Init Falied."<<endl;
        return false;
    }
    cout<<"Table '"<<__Table->__TableName<<"' Init Success."<<endl;
    return true;
}


bool Memorizer::storage(){
    if(__Table == NULL) return false;
    string filePath = __HOME_DIRECTORY__ + __Table->__TableName + __FILE_SUFFIX__;
    FILE* __FilePtr = fopen(filePath.c_str(),"ab+");
    fclose(__FilePtr);
    try{
        __FilePtr = fopen(filePath.c_str(),"r+");
        fseek(__FilePtr,0,SEEK_SET);
        fwrite(&__Table->__TableID,sizeof(int),1,__FilePtr);
        char temp[TEXT_LENGTH];
        memset(temp,'\0',sizeof(temp));
        strcpy(temp,__Table->__TableName.c_str());
        fwrite(&temp,sizeof(char)*TEXT_LENGTH,1,__FilePtr);
        fwrite(&__Table->__TableHeadSize,sizeof(__uint16_t),1,__FilePtr);
        ///////////////////////////////////////////////////////////////////////////////
        fwrite(&__Table->__TotalPage,sizeof(__uint16_t),1,__FilePtr);
        fwrite(&__Table->__EmptyPage,sizeof(__uint16_t),1+MAX_EMPTY_PAGE,__FilePtr);
        ////////////////////////////////////////////////////////////////////////////////
        fwrite(&__Table->__ParmNum,sizeof(__uint16_t),1,__FilePtr);
        for(int i = 0; i < __Table->__ParmNum; i++){
            __uint16_t type = __Table->__ParmType[i];
            fwrite(&type,sizeof(__uint16_t),1,__FilePtr);
            memset(temp,'\0',sizeof(temp));
            strcpy(temp,__Table->__ParmName[i].c_str());
            fwrite(&temp,sizeof(char)*TEXT_LENGTH,1,__FilePtr);
        }
        fwrite(&__Table->__Key,sizeof(__uint16_t),1,__FilePtr);
        fwrite(&__Table->__RowTakeup,sizeof(__uint16_t),1,__FilePtr);
        fwrite(&__Table->__MaxRowPerPage,sizeof(__uint16_t),1,__FilePtr);
        fclose(__FilePtr);
    }catch(exception &e){
        return false;
    }
    return true;
}

bool Memorizer::update(){
    if(__Table == NULL) return false;
    string filePath = __HOME_DIRECTORY__ + __Table->__TableName + __FILE_SUFFIX__;
    try{
        FILE* __FilePtr = fopen(filePath.c_str(),"r+");
        fseek(__FilePtr,TABLE_UPDATE_OFFSET,SEEK_SET);
        fwrite(&__Table->__TotalPage,sizeof(__uint16_t),1,__FilePtr);
        fwrite(&__Table->__EmptyPage,sizeof(__uint16_t),1+MAX_EMPTY_PAGE,__FilePtr);
        fclose(__FilePtr);
    }catch(exception &e){
        return false;
    }
    return true;
}

bool Memorizer::page_read_in(__uint16_t page_offset){
    string filePath = __HOME_DIRECTORY__ +  __Page->t->__TableName + __FILE_SUFFIX__;
    try{
        FILE* __ReadPtr = fopen(filePath.c_str(),"rb");
        __uint64_t bytes_offset = __Page->t->__TableHeadSize + page_offset * BYTES_PER_PAGE;
        fseek(__ReadPtr,bytes_offset,SEEK_SET);
        fread(&(__Page->__PageOffset),sizeof(__uint16_t),1,__ReadPtr);
        Index* index = new Index(__Page->t->getKeyType());
        fread(&(index->INDEX),sizeof(__INDEX__),1,__ReadPtr);
        __Page->__PageIndex = *index;
        fread(&(__Page->__IsFull),sizeof(bool),1,__ReadPtr);
        fread(&__Page->__CursorOffset,sizeof(__uint16_t),1,__ReadPtr);
        ///////////////////////////////////////////////////////////////////////
        for(int i = 0; i < __Page->t->__MaxRowPerPage; i++){
            Row* new_row = new Row(__Page->t);
            __Page->__Rows[i] = new_row;
            for(int i = 0; i < __Page->t->__ParmNum; i++){
                fread(new_row->__RowContent[i],__TypeSize__[__Page->t->getKeyType()],1,__ReadPtr);
                new_row->index_update();
            }
        }
        fclose(__ReadPtr);

    }catch(exception &e){
        return false;
    }
    return true;
}

bool Memorizer::page_flush(__uint16_t page_offset){
    if(__Page == NULL) return false;
    string filePath = __HOME_DIRECTORY__ +  __Page->t->__TableName + __FILE_SUFFIX__;
    FILE* __FilePtr = fopen(filePath.c_str(),"ab+");
    fclose(__FilePtr);
    try{
        __uint64_t bytes_offset = __Page->t->__TableHeadSize + page_offset * BYTES_PER_PAGE;
        FILE* __WritePtr = fopen(filePath.c_str(),"r+");
        fseek(__WritePtr,bytes_offset,SEEK_SET);
        char ch = '\0';
        fwrite(&ch,BYTES_PER_PAGE,1,__WritePtr);
        fclose(__WritePtr);
    }catch(exception &e){
        return false;
    }
    return true;
}

bool Memorizer::page_write_back(__uint16_t page_offset){
    if(__Page == NULL) return false;
    string filePath = __HOME_DIRECTORY__ +  __Page->t->__TableName + __FILE_SUFFIX__;
    FILE* __FilePtr = fopen(filePath.c_str(),"ab+");
    fclose(__FilePtr);
    try{
        __uint64_t bytes_offset = __Page->t->__TableHeadSize + page_offset * BYTES_PER_PAGE;
        FILE* __WritePtr = fopen(filePath.c_str(),"r+");
        fseek(__WritePtr,bytes_offset,SEEK_SET);
        ////////////////////////////////////////////////////////////
        fwrite(&(__Page->__PageOffset),sizeof(__uint16_t),1,__WritePtr);
        fwrite(&(__Page->__PageIndex.INDEX),sizeof(__INDEX__),1,__WritePtr);
        fwrite(&__Page->__IsFull,sizeof(bool),1,__WritePtr);
        fwrite(&__Page->__CursorOffset,sizeof(__uint16_t),1,__WritePtr);
        ////////////////////////////////////////////////////////////////////////
        for(__uint16_t i = 0; i < __Page->__CursorOffset; i++){
            Row* cur_row = __Page->__Rows[i];
            for(int i = 0; i < __Page->t->__ParmNum; i++){
                fwrite(cur_row->__RowContent[i],__TypeSize__[__Page->t->__Key],1,__WritePtr);
            }
        }
        fclose(__WritePtr);

    }catch(exception &e){
        return false;
    }
    return true; 
}