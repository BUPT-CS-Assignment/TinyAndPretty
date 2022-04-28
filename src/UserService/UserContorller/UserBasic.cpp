#include <UserService/UserControl.h>
#include <UserService/Preload.h>
using namespace NEDBSTD;
using namespace UTILSTD;
using namespace std;

User::User(){
    id_ = 0;
    auth_ = 0;
    status_ = 0;
    UserDB = NULL;
}

User::User(int id, int auth){
    id_ = id;
    auth_ = auth;
    UserDB = NULL;
    status_ = USER_SIGN_OUT;
}

int User::SignIn(std::string passwdInput){
    std::string id_s = std::to_string(id_);
    // Signing In //
    int count;
    string retVal;
    int errCode = PRELOAD_DB.Select("UserToken", "Passwd", "ID=" + to_string(id_), count, retVal);
    if(errCode != NO_ERROR){
        return errCode;
    }
    retVal = retVal.substr(retVal.find(";") + 1);
    if(passwdInput != retVal){
        return -1;
    }
    status_ = USER_SignIn;
    std::string userDir = USER_DIR + "/" + id_s;
    if(UserDB == NULL){
        UserDB = new NEDB(userDir.c_str());
        UserDB->DirInit();
    }
    return CONSOLE_LOG(0, 1, 1, "User '%d' Signed In\n", id_);
}

int User::SignUp(std::string passwdInput){
    std::string id_s = std::to_string(id_);
    int errCode1 = PRELOAD_DB.Insert("UserToken", "", id_s + "," + passwdInput + ",0");
    if(errCode1 != NO_ERROR){
        return errCode1;
    }
    int errCode2 =  PRELOAD_DB.Insert("UserInfo", "ID", id_s);
    if(errCode2 == NO_ERROR){
        std::string userDir = USER_DIR + "/" + id_s;
        mkdir(userDir.c_str(), S_IRWXU);
        return CONSOLE_LOG(0,1,1,"User Dir '%s' Created\n", userDir.c_str());
    }
    return errCode2;
}

int User::SignOut(){
    status_ = USER_SIGN_OUT;
    UserDB->Close();
    UserDB = NULL;
    COOKIE.DeleteData(id_);
    return 0;
}

std::string User::Format(){
    int count;
    string retVal;
    int errCode = PRELOAD_DB.Select("UserInfo","*","ID="+ std::to_string(id_),count,retVal);
    if(errCode != NO_ERROR) return "";
    retVal = retVal.substr(retVal.find(";") + 1);
    int length;
    string* str = Split(retVal, ',', length);

    /* Return ID and Name */
    string resp = str[0] + "," + str[1] + "," +str[2] + ",";

    /* Return School */
    errCode = PRELOAD_DB.Select("SchoolInfo","Name","ID="+str[3],count,retVal);
    if(errCode != NO_ERROR) return "";
    resp += retVal.substr(retVal.find(";") + 1) + ",";

    /* Return Major */
    errCode = PRELOAD_DB.Select("MajorInfo","Name","ID="+str[4],count,retVal);
    if(errCode != NO_ERROR) return "";
    resp += retVal.substr(retVal.find(";") + 1) + ",";

    /* Return Class */
    resp += str[5];
    delete[] str;
    return resp;
}

int User::SetInfo(std::string value){
    int count;
    int errCode = PRELOAD_DB.Update("UserInfo",value,"ID="+to_string(id_),count);
    return errCode;
}

string User::GetInfo(string value){
    int count;
    string res;
    int errCode = PRELOAD_DB.Select("UserInfo",value,"ID="+to_string(id_),count,res);
    if(errCode != NO_ERROR) return "";
    res = res.substr(res.find(";") + 1);
    return res;
}
