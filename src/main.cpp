#include <common.h>


int main(int argv , char* argc[]) {
    TPHttpServer server(9006 , 5);
    server.start();
    server.stop();
    return 0;
}