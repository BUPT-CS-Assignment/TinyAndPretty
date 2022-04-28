#include <UserService/Preload.h>
#include <UserService/UserControl.h>
using namespace std;
using namespace NEDBSTD;
using namespace BTREESTD;

NEDB PRELOAD_DB("/");
BalanceTree<User, int> COOKIE(0);
std::string PROJECT_DIR = "/";
std::string PRELOAD_DIR = "/";
std::string USER_DIR = "/";
std::string COURSE_DIR = "/";
std::string CLASS_DIR = "/";

int ServicePreLoad(){
    /* Set DataBase Basic Info */
    NEDB_SETTING(256, 400, 50, 5);
    NEDB_DEBUG(2);
    NEDB_TIME_FLAG(true);
    /////
    PRELOAD_DB.SetDir(PRELOAD_DIR.c_str());
    if(PRELOAD_DB.DirInit() != 0){
        return UTILSTD::CONSOLE_LOG(3,1,1,"Dir Error\n");
    }
    int num;
    PRELOAD_DB.MountAll(num);
    return UTILSTD::CONSOLE_LOG(0,1,1," %d Table Mounted\n",num);
}

void DirectoryInit(){
    PROJECT_DIR = get_current_dir_name();
    PRELOAD_DIR = PROJECT_DIR + "/data/Preload";
    USER_DIR = PROJECT_DIR + "/data/User";
    COURSE_DIR = PROJECT_DIR + "/data/Course";
    CLASS_DIR = PROJECT_DIR + "/data/Class";
}
