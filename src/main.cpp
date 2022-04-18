#include <common.h>
#include <Network/TAPManager.h>
#include <HttpProtocal/HttpManager.h>

int main(int argv , char* argc[]) {
    TAPManager server;

    server.loadSubManager( std::make_unique<HttpManager>() );
    
    server.start();
    return 0;
}