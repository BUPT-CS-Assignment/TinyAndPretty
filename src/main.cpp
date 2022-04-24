#include <common.h>
#include <Network/TAPManager.h>
#include <HttpProtocal/HttpManager.h>

#include <libs/NEDB.h>
NEDB DB("/home/jianxf/.nesrc");

int main(int argv , char* argc[]) {
    NEDB_DEBUG(1);
    DB.DirInit();
    DB.Openall();
    
    TAPManager server;
    server.loadSubManager( std::make_unique<HttpManager>() );
    server.start();
    return 0;
}