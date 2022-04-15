#ifndef __HTTP_SERVER__
#define __HTTP_SERVER__

#include <common.h>
#include <interfaces.h>
#include "ThreadPool.h"
#include "ServerBase.h"

class HttpServer {
private :  
    std::unique_ptr<Socket>     sock;
    std::unique_ptr<EventPool>  epool;
    std::unique_ptr<ThreadPool> thpool;
public :
    HttpServer();
    void start();
    void createHttpTask(Connection *conn);
};
#endif