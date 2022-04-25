#include <common.h>
#include <Network/TAPManager.h>
#include <HttpProtocal/HttpManager.h>
#include <test/define.h>

#include <libs/NEDB.h>
NEDB DB("/home/jianxf/.nesrc");
std::string __SQL__ = "";

int main(int argv , char* argc[]) {
    /* SQL Test Init */
    for(int i = 0;i<100;i++){
        __SQL__ += "insert into test2 values ("+std::to_string(i)+");";
    }
    for(int i = 30;i<60;i++){
        __SQL__ += "delete from test2 where id = "+std::to_string(i)+";";
    }
    __SQL__ += "select * from test2;";

    /* DataBase Init */
    NEDB_SETTING(256,400,200,5);
    NEDB_DEBUG(1);
    DB.DirInit();
    DB.Openall();
    
    /* Server Init */
    TAPManager server;
    server.loadSubManager( std::make_unique<HttpManager>() );
    server.start();
    return 0;
}