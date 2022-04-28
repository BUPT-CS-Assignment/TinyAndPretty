#include <common.h>
#include <BasicService/Network/TAPManager.h>
#include <BasicService/HttpProtocal/HttpManager.h>
#include <UserService/Preload.h>

int main(int argv, char* argc[]){
    /* Dir Init */
    DirectoryInit();
    /* Server Init */
    UTILSTD::CONSOLE_LOG(0,1,1,"Project Directory '%s'\n",PROJECT_DIR.c_str());
    SQLTestGenerate();
    ServicePreLoad();
    TAPManager server;
    server.loadSubManager(std::make_unique<HttpManager>());
    server.start();
    return 0;
}
