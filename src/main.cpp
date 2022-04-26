#include <common.h>
#include <Network/TAPManager.h>
#include <HttpProtocal/HttpManager.h>
#include <UserService/Preload.h>

int main(int argv, char* argc[]){
    /* Dir Init */
    PROJECT_DIR = get_current_dir_name();

    /* Server Init */
    SQLTestGenerate();
    ServicePreLoad();
    TAPManager server;
    server.loadSubManager(std::make_unique<HttpManager>());
    server.start();
    return 0;
}
