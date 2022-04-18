#include <common.h>
#include <Network/TAPManager.h>
#include <HttpProtocal/HttpManager.h>

#include <libs/NEdb.h>
NEdb DB("/home/jianxf/.nesrc");

int main(int argv , char* argc[]) {
    DB.dirInit();
    DB.scan();
    
    TAPManager server;
    server.loadSubManager( std::make_unique<HttpManager>() );
    server.start();
    return 0;
}