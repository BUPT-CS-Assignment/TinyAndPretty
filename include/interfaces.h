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
#include <connect/HttpProtocal/HttpProtocal.h>
#include <connect/HttpProtocal/HttpException.h>

#include <connect/Network/TAPManager.h>
#include <libs/SimpleJson.hpp>
using namespace SimpleJson;

////simplize entry define 
#define def_HttpEntry(name , request) extern HttpResponseBase* (name)(HttpRequest &request) 


/*---------------- Hello-world Module ----------------*/
def_HttpEntry(Link_Start , request);
def_HttpEntry(Lent_Book , req);
def_HttpEntry(Icon , req);
def_HttpEntry(Check_It , req);

/*---------------------MD5 Module---------------------*/
#include <libs/md5.h>
def_HttpEntry(MD5Test,req);

/*----------------- Data-Base Module -----------------*/
#include <libs/NEDB.h>
#include <libs/BalanceTree.h>
#include <libs/BasicUtils.hpp>

/* User Service */
def_HttpEntry(SignIn,req);
def_HttpEntry(SignUp,req);
/* DataBase Test */
def_HttpEntry(SqlTest,req);
def_HttpEntry(SqlRun , req);
def_HttpEntry(SqlHelp ,  req);

def_HttpEntry(UserIndex,req);
def_HttpEntry(UserScheduleIndex,req);


def_HttpEntry(LogCheck,req);


#endif