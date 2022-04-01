#include <common.h>
#include <interfaces.h>
#include <nedb.h>

int main(int argv , char* argc[]) {
    nedb* db = NULL;
    char* msg = NULL;
    nedb_auto(&db,&msg);
    printf("%s\n",msg);
    HttpServer server(9006 , 5);
    server.start();
    return 0;
}