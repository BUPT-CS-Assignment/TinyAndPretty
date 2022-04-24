#ifndef __STORAGE_H__
#define __STORAGE_H__
#include<main.h>
#include<Basic/data.h>

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
    Table* table_;
public:
    Memorizer(Table* table);
    Table*  TableLoad(DataBase*,string);
    void    TableStore();
    void    TableUpdate();
    /////////////////////////
    bool    storage();
    bool    update();
    void    PageFlush(__uint16_t);
    Page*   PageLoad(__uint16_t);
    void    PageStore(__uint16_t, Page*);
    void    TableDrop();
    
    
};


#endif