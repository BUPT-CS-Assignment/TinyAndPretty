#include <common.h>
#include <connect/Network/TAPManager.h>
#include <connect/HttpProtocal/HttpManager.h>
#include <connect/Timer/TimerManager.h>
#include <service/TAPSystem.h>

int main(int argv, const char* argc[]){
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
    server.loadSubManager(std::make_unique<TimerManager>());
    server.start();

    return 0;
}
