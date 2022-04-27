#include <UserService/Preload.h>
#include <UserService/UserControl.h>
using namespace std;

NEDB DB_ROOT("/");
BalanceTree<User,int> UserStatus(0);
std::string PROJECT_DIR = "/";

int ServicePreLoad(){
    /* Set DataBase Basic Info */
    NEDB_SETTING(256,400,50,5);
    NEDB_DEBUG(2);
    /////
    string data_dir = PROJECT_DIR + PRELOAD_DIR;
    DB_ROOT.SetDir(data_dir.c_str());
    if(DB_ROOT.DirInit() != 0){
        printf("Dir Error\n");
        return 3;
    }
    DB_ROOT.Openall();
    return 0;
}

