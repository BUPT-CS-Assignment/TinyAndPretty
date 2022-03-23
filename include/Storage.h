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
 * 
 */
class Memorizer{
private:
    Table* __Table;
    Page* __Page;
public:
    Memorizer();
    Memorizer(Table*);
    Memorizer(Page*);
    Memorizer(Table*,Page*);
    void change_page(Page*);
    bool load(string);
    bool storage();
    bool update();
    bool page_flush(__uint16_t page_offset);
    bool page_read_in(__uint16_t page_offset);
    bool page_write_back(__uint16_t page_offset);

};


#endif