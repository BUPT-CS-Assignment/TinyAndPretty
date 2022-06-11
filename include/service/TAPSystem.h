#ifndef __SERVER_SYS_H__
#define __SERVER_SYS_H__
#include <common.h>
#include <interfaces.h>


extern NEDBSTD::NEDB __DATABASE;   //系统资源数据库
extern std::string PROJECT_DIR; //项目路径
extern std::string SYS_DIR;     //系统路径
extern std::string USER_DIR;    //用户路径
extern std::string SRC_DIR;     //资源路径
extern std::string TIME_STAMP;  //启动时间

#define TOKEN_ACCESS 1
#define TOKEN_DENIED 0

void TimeStampReset();
void DirectoryInit();
int ServicePreload();
void SQLTestGenerate();

std::string TokenSign(std::string userid);
int TokenCheck(std::string userid,std::string token);

#endif