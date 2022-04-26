#ifndef __MAIN_INTERFACE__
#define __MAIN_INTERFACE__

/*============== SUBMODULE INTERFACES ==============*/
/*
    In this head file , you shall make sure your submodule 
    interfaces be included , so that the main module could
    invoke your submodule through the entry function defined
    in 'router.conf' 
*/

/*----------------- Network Module ----------------- */
#include <HttpProtocal/HttpProtocal.h>
#include <HttpProtocal/HttpException.h>

#include <Network/TAPManager.h>
#include <libs/SimpleJson.hpp>
using namespace SimpleJson;

////simplize entry define 
#define def_HttpEntry(name , request) extern HttpResponseBase* (name)(HttpRequest &request) 


/*---------------- Hello-world Module ----------------*/
def_HttpEntry(Link_Start , request);
def_HttpEntry(Lent_Book , req);
def_HttpEntry(ICON , req);
def_HttpEntry(Check_It , req);

/*---------------------MD5 Module---------------------*/
#include <libs/md5.h>
def_HttpEntry(MD5_Test,req);

/*----------------- Data-Base Module -----------------*/
#include <libs/NEDB.h>
extern NEDB DB;
/* User Service */
def_HttpEntry(LOG_IN,req);
def_HttpEntry(SIGN_UP,req);
/* DataBase Test */
def_HttpEntry(SQL_Test,req);
def_HttpEntry(SQL_Test2,req);
def_HttpEntry(SQL_Run , req);
def_HttpEntry(SQL_HELP ,  req);



#endif