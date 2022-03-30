#ifndef __NEDB_H__
#define __NEDB_H__

extern "C" {
#define __HomeDir__ "/home/jianxf/.nesrc/tables/"
#define MAX_TABLES 50

typedef struct nedb nedb;
int nedb_auto_start(nedb**,char* msg);
int nedb_open(nedb**,const char* name,char** msg);
int nedb_exec(nedb*,const char* sql,char** data,char** msg);
int nedb_close(nedb*);
}

#endif