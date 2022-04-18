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
#define def_HttpEntry(name) HttpResponseBase* (name)(HttpRequest &request) 
#include <HttpProtocal/HttpProtocal.h>
#include <HttpProtocal/HttpException.h>

#include <Network/TAPManager.h>
#include <libs/SimpleJson.hpp>
using namespace SimpleJson;


/*----------------- Hello-World Module ----------------- */
def_HttpEntry(Link_Start);
def_HttpEntry(Lent_Book);

#endif