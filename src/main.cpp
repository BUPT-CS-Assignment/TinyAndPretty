#include <common.h>
#include <connect/Network/TAPManager.h>
#include <connect/HttpProtocal/HttpManager.h>
#include <service/sys/ServerSys.h>

int main(int argv, char* argc[]){
    //Time Stamp Set
    TimeStampReset();
    //Dir Init
    DirectoryInit();
    UTILSTD::CONSOLE_LOG(0,1,1,"Project Directory '%s'\n",PROJECT_DIR.c_str());
    //SQL Test Init
    SQLTestGenerate();
    //Service Resource Init
    ServicePreload();
    //Server Init
    TAPManager server;
    server.loadSubManager(std::make_unique<HttpManager>());
    server.start();
    return 0;
}
