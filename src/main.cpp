#include <common.h>
#include <interfaces.h>

int main(int argv , char* argc[]) {

    HttpServer server(9006);
    
    server.start();
    return 0;
}