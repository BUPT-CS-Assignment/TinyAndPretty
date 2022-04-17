#include <common.h>
#include <Network/HttpServer.h>
NEdb DB("/home/jianxf/.nesrc");

int main(int argv , char* argc[]) {
    DB.dirInit();
    DB.scan();
    HttpServer server;
    server.start();
    return 0;
}