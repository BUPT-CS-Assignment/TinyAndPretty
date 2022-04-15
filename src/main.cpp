#include <common.h>
#include <Network/HttpServer.h>

int main(int argv , char* argc[]) {
    HttpServer server;
    server.start();
    return 0;
}