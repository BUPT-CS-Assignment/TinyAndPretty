#include <service/user/UserControl.h>
#include <service/sys/ServerSys.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;


int UserSignIn(string userid, std::string passwdInput){
    int count;
    string retVal;
    int errCode = __LSR__.Select("token", "passwd", "id=" + userid, count, retVal);
    if(errCode != 0){
        return CONSOLE_LOG(errCode, 1, 1,"SQL-Error '%s'\n",NEexceptionName[errCode].c_str());
    }
    retVal = retVal.substr(retVal.find(";") + 1);
    if(passwdInput != retVal){
        return -1;
    }
    return CONSOLE_LOG(0, 1, 1, "User '%s' Signed In\n", userid.c_str());
}

int UserSignUp(string userid, string passwdInput){
    int errCode1 = __LSR__.Insert("token", "", userid + "," + passwdInput + ",0");
    if(errCode1 != NO_ERROR){
        return CONSOLE_LOG(errCode1, 1, 1,"SQL-Error '%s'\n",NEexceptionName[errCode1].c_str());
    }
    int errCode2 = __LSR__.Insert("users", "id", userid);
    return CONSOLE_LOG(errCode2, 1, (errCode2 == NO_ERROR), "User '%s' Signed Up\n", userid);
}

string UserInfoFetch(string userid){
    int count;
    string retVal;
    int errCode = __LSR__.Select("users", "*", "id=" + userid, count, retVal);
    if(errCode != NO_ERROR) return "";
    int length;
    string* str = Split(retVal.substr(retVal.find(";") + 1), ',', length);
    string info = str[0] + "," + str[1] + "," + (str[2] == "0" ? "女," : "男,");
    errCode = __LSR__.Select("schools", "name", "id=" + str[3], count, retVal);
    info += retVal.substr(retVal.find(";") + 1) + ",";
    errCode = __LSR__.Select("majors", "name", "id=" + str[4], count, retVal);
    info += retVal.substr(retVal.find(";") + 1) + ",";
    info += str[5];
    delete[] str;
    return info;
}

int UserInfoUpdate(string userid,string value){
    int count;
    int errCode = __LSR__.Update("users",value,"id="+userid,count);
    return errCode;
}


