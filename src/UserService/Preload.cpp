#include <UserService/Preload.h>
#include <UserService/UserControl.h>
using namespace std;

NEDB MainDB("/");
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
    /////
    MainDB.SetDir(PRELOAD_DIR.c_str());
    if(MainDB.DirInit() != 0){
        printf("Dir Error\n");
        return 3;
    }
    MainDB.Openall();
    return 0;
}

void DirectoryInit(){
    PROJECT_DIR = get_current_dir_name();
    PRELOAD_DIR = PROJECT_DIR + "/data/Preload";
    USER_DIR = PROJECT_DIR + "/data/User";
    COURSE_DIR = PROJECT_DIR + "/data/Course";
    CLASS_DIR = PROJECT_DIR + "/data/Class";
}
