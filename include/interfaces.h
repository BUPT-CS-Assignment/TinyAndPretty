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
#include <BasicService/HttpProtocal/HttpProtocal.h>
#include <BasicService/HttpProtocal/HttpException.h>

#include <BasicService/Network/TAPManager.h>
#include <BasicService/libs/SimpleJson.hpp>
using namespace SimpleJson;

////simplize entry define 
#define def_HttpEntry(name , request) extern HttpResponseBase* (name)(HttpRequest &request) 


/*---------------- Hello-world Module ----------------*/
def_HttpEntry(Link_Start , request);
def_HttpEntry(Lent_Book , req);
def_HttpEntry(ICON , req);
def_HttpEntry(Check_It , req);

/*---------------------MD5 Module---------------------*/
#include <BasicService/libs/md5.h>
def_HttpEntry(MD5_Test,req);

/*----------------- Data-Base Module -----------------*/
#include <BasicService/libs/NEDB.h>
#include <BasicService/libs/BalanceTree.h>
#include <BasicService/libs/BasicUtils.hpp>

/* User Service */
def_HttpEntry(SIGN_IN,req);
def_HttpEntry(SIGN_UP,req);
/* DataBase Test */
def_HttpEntry(SQL_TEST,req);
def_HttpEntry(SQL_RUN , req);
def_HttpEntry(SQL_HELP ,  req);

def_HttpEntry(UserIndex,req);
def_HttpEntry(UserTask,req);
def_HttpEntry(UserInfo,req);



#endif