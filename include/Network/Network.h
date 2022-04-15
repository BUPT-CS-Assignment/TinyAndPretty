#ifndef __NET__
#define __NET__

// Json Module
#include <libs/SimpleJson.hpp>
using namespace SimpleJson;

// Http protocal
#include "HttpProtocal.h"

#define def_HttpEntry(name) HttpResponseBase* (name)(HttpRequest &request) 

#endif