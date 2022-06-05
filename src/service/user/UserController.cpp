#include <service/user/UserControl.h>
#include <service/sys/ServerSys.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;


User::User(string id){
    this->id = id;
    auth = USER_PERSON;
}

string User::getID(){
    return id;
}

void User::setAuth(int auth){
    this->auth = auth;
}

int User::Signin(string& passwd){
    int count;
    string retVal;
    int errCode = __LSR__.Select("token", "passwd", "id=" + id, count, retVal);
    if(errCode != 0){
        return CONSOLE_LOG(errCode, 1, 1, "SQL-Error '%s'\n", NEexceptionName[errCode].c_str());
    }
    retVal = retVal.substr(retVal.find(";") + 1);
    if(passwd != retVal){
        return -1;
    }
    Init();
    if(auth < 3 && (schoolid == "0" || classid == "0")){
        return -1;
    }
    //info preload
    return CONSOLE_LOG(0, 1, 1, "User '%s' Signed In\n", id.c_str());
}

void User::Init(){
    int count,length;
    string retVal;
    int errCode = __LSR__.Select("users","*","id=" + id, count, retVal);
    if(errCode != NO_ERROR) return;
    retVal = retVal.substr(retVal.find_first_of(';')+1);
    string * str = Split(retVal,',',length);
    auth = stoi(str[1]);
    name = str[2];
    gender = (str[3] == "0" ? "女" : "男");
    schoolid = str[4];
    majorid = str[5];
    classid = str[6];
    delete[] str;
}

int User::Signup(string& passwd){
    int errCode1 = __LSR__.Insert("token", "", id + "," + passwd + ",0");
    if(errCode1 != NO_ERROR){
        return CONSOLE_LOG(errCode1, 1, 1, "SQL-Error '%s'\n", NEexceptionName[errCode1].c_str());
    }
    int errCode2 = __LSR__.Insert("users", "id,auth", id + "," + to_string(auth));
    return CONSOLE_LOG(errCode2, 1, (errCode2 == NO_ERROR), "User '%s' Signed Up\n", id);
}

Json User::getInfo(){
    int count;
    string retVal;
    Json J;
    J.push_back({"id",stoi(id)});
    J.push_back({"auth",auth});
    J.push_back({"name",name.c_str()});
    J.push_back({"gender",gender.c_str()});
    //school
    __LSR__.Select("schools", "name", "id=" + schoolid, count, retVal);
    J.push_back({"school",retVal.substr(retVal.find(";") + 1).c_str()});
    //major
    __LSR__.Select("majors", "name", "id=" + majorid, count, retVal);
    J.push_back({"major",retVal.substr(retVal.find(";") + 1).c_str()});
    //class
    J.push_back({"classid",stoi(classid)});
    return J;
}

int User::setInfo(string& value){
    int count;
    int errCode = __LSR__.Update("users", value, "id=" + id, count);
    return errCode;
}

string User::getName(){
    return name;
}

int User::getAuth(){
    return auth;
}

string User::getGender(){
    return gender;
}

string User::getSchool(){
    return schoolid;
}

string User::getMajor(){
    return majorid;
}

std::string User::getClass(){
    return classid;
}




