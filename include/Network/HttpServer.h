#ifndef __HTTP_SERVER__
#define __HTTP_SERVER__

#include <common.h>
#include <interfaces.h>
#include "ServerBase.h"

class HttpServer {
private :  
    std::unique_ptr<Socket>     sock;
    std::unique_ptr<EventPool>  epool;
public :
    HttpServer(uint16_t _port);
    void start();
    void createHttpTask(Connection *conn);
};
#endif