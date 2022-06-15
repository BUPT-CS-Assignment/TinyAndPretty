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


/*------------------- Timer Module -------------------*/
#include <connect/Timer/Timer.h>

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

/*----------------- Vue/Axios Module-----------------*/

def_HttpEntry(Main,req);

/* User Service */
def_HttpEntry(API_Signin,req);
def_HttpEntry(API_Signup,req);

/* Access Check */
def_HttpEntry(API_Access,req);
/* DataBase Test */
def_HttpEntry(API_SQL_Test,req);
def_HttpEntry(API_SQL , req);
def_HttpEntry(API_SQL_Help ,  req);

def_HttpEntry(API_Course, req);
def_HttpEntry(API_File,req);

def_HttpEntry(API_Class,req);
def_HttpEntry(API_School,req);
def_HttpEntry(API_Major,req);

def_HttpEntry(API_User,req);
def_HttpEntry(API_Timetable,req);
def_HttpEntry(API_Event,req);

def_HttpEntry(LogCheck,req);


def_HttpEntry(API_Clock , req);

def_HttpEntry(MapTest ,req);


def_HttpEntry(FileUpload , req);
#endif