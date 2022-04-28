#ifndef __PRELOAD_H__
#define __PRELOAD_H__
#include <common.h>
#include <interfaces.h>

extern NEDBSTD::NEDB PRELOAD_DB;
extern std::string DataBaseTestSQL;
extern std::string PROJECT_DIR;
extern std::string PRELOAD_DIR;
extern std::string USER_DIR;
extern std::string COURSE_DIR;
extern std::string CLASS_DIR;

void DirectoryInit();
int ServicePreLoad();
void SQLTestGenerate();

#endif
