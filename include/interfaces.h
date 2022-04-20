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

////simplize entry define 
#define def_HttpEntry(name) HttpResponseBase* (name)(HttpRequest &request) 

/*----------------- Data-Base Module ---------------- */
// #include <libs/NEdb.h>
// extern NEdb DB;

/*---------------- Hello-world Module --------------- */
def_HttpEntry(Link_Start);
def_HttpEntry(Lent_Book);
def_HttpEntry(SQL_Run);
def_HttpEntry(SQL_HELP);
def_HttpEntry(ICON);


#endif