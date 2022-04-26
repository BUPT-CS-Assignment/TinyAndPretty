#ifndef __PRELOAD_H__
#define __PRELOAD_H__
#include <common.h>
#include <interfaces.h>

extern NEDB DB_ROOT;
extern std::string DataBaseTestSQL;
int ServicePreLoad();
void SQLTestGenerate();

#endif
