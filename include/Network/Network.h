#ifndef __NET__
#define __NET__

#include "HttpServer.h"

// Json Module
#include "SimpleJson.hpp"
using namespace SimpleJson;

// Http protocal
#include "HttpProtocal.h"

#define def_HttpEntry(name) HttpResponseBase* (name)(HttpRequest &request) 

#endif