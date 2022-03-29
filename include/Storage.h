#ifndef __STORAGE_H__
#define __STORAGE_H__
#include<Basic.h>
#include<Data.h>

/**
 * FILE 文件存储
 * 
 * FRMMAT
 * --------------------
 * table_name.db        //表名
 * parm_num id \n type \n ...
 * row_number 
 * 
 * 
 * 
 * when open, load in all pointers of files;
 * if loacte ptr* of files;
 * readin 
 * operte in memory
 * wirte back
 * free meomery
 * call pointer at the head of the file;
 * 
 */

class Memorizer{
public:
    Memorizer();
    Table* TableLoad(string);
    void TableStore(Table*);
    void TableUpdate(Table*);
    void IndexLoad();
    void IndexStore();
    void IndexUpdate();
    /////////////////////////
    bool storage();
    bool update();
    void PageFlush(__uint16_t, Table*);
    Page* PageLoad(__uint16_t, Table*);
    void PageStore(__uint16_t, Page*);
    
    
};


#endif