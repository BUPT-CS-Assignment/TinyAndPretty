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
def_HttpEntry(Icon , req);
def_HttpEntry(Check_It , req);

/*---------------------MD5 Module---------------------*/
#include <BasicService/libs/md5.h>
def_HttpEntry(MD5Test,req);

/*----------------- Data-Base Module -----------------*/
#include <BasicService/libs/NEDB.h>
#include <BasicService/libs/BalanceTree.h>
#include <BasicService/libs/BasicUtils.hpp>

/* User Service */
def_HttpEntry(SignIn,req);
def_HttpEntry(SignUp,req);
/* DataBase Test */
def_HttpEntry(SqlTest,req);
def_HttpEntry(SqlRun , req);
def_HttpEntry(SqlHelp ,  req);

def_HttpEntry(UserIndex,req);
def_HttpEntry(UserTask,req);
def_HttpEntry(UserInfo,req);

def_HttpEntry(LogCheck,req);


#endif