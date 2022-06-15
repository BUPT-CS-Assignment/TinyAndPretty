#include <service/User.h>
#include <service/TAPSystem.h>
#include <service/Event.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;


User::User(string id){
    this->id = id;
    auth = USER_COMMON;
}

int User::Signin(string& passwd){
    int count;
    string retVal;
    int errCode = __DATABASE.Select("token", "passwd", "id=" + id, count, retVal);
    if(errCode != 0){
        return CONSOLE_LOG(errCode, 1, 1, "SQL-Error '%s'\n", NEexceptionName[errCode].c_str());
    }
    retVal = retVal.substr(retVal.find(";") + 1);
    if(passwd != retVal){
        return -1;
    }
    Query();
    if(auth < USER_SCHOOL && (schoolid == "0" || classid == "0")){
        return -2;
    }
    //info preload
    return CONSOLE_LOG(0, 1, 1, "User '%s' Signed In\n", id.c_str());
}

int User::Signup(string& passwd){
    int errCode1 = __DATABASE.Insert("token", "", id + "," + passwd + ",0");
    if(errCode1 != NO_ERROR){
        return CONSOLE_LOG(errCode1, 1, 1, "SQL-Error '%s'\n", NEexceptionName[errCode1].c_str());
    }
    int errCode2 = __DATABASE.Insert("users", "id,auth", id + "," + to_string(auth));
    return CONSOLE_LOG(errCode2, 1, (errCode2 == NO_ERROR), "User '%s' Signed Up\n", id);
}

int User::Update(string& value){
    int count;
    return __DATABASE.Update("users", value, "id=" + id, count);
}

int User::Query(bool d_name){
    int count, length;
    string retVal;
    int errCode = __DATABASE.Select("users", "*", "id=" + id, count, retVal);
    if(errCode != NO_ERROR){
        return errCode;
    }
    retVal = retVal.substr(retVal.find_first_of(';') + 1);
    string* str = Split(retVal, ',', length);
    if(length != 7){
        delete[] str;
        return PARAM_FORM_ERROR;
    }
    auth = stoi(str[1]);
    name = str[2];
    gender = (str[3] == "0" ? "女" : "男");
    schoolid = str[4];
    majorid = str[5];
    classid = str[6];
    delete[] str;
    if(d_name){
        //School Name
        __DATABASE.Select("schools", "name", "id=" + schoolid, count, retVal);
        schoolName = retVal.substr(retVal.find(";") + 1);

        //Major Name
        __DATABASE.Select("majors", "name", "id=" + majorid, count, retVal);
        majorName = retVal.substr(retVal.find(";") + 1);
    }
    return NO_ERROR;
}

Json User::Format(){
    Json J;
    J.push_back({"id",stoi(id)});
    J.push_back({"auth",auth});
    J.push_back({"name",name});
    J.push_back({"email","test@noui.cloud"});
    J.push_back({"gender",gender});
    J.push_back({"schoolid",schoolid});
    J.push_back({"school",schoolName});
    J.push_back({"majorid",majorid});
    J.push_back({"major",majorName});
    J.push_back({"classid",classid});
    return J;
}

Json User::getTimeTable(){
    Class c(classid);
    if(auth == USER_SCHOOL){
        return c.getTimeTable(schoolid,id);
    }else{
        return c.getTimeTable(schoolid);
    }
    
}

Json User::getEvents(){
    int count;
    string retVal;
    NEDB _DB(USER_DIR + "/" + schoolid + "/" + classid + "/" + id);
    _DB.Mount("event");
    int errCode = _DB.Select("event", "*", "", count, retVal);
    _DB.Close();

    //Manage Info
    int length;
    Json J;
    string* str = Split(retVal, ';', length);
    if(errCode != NO_ERROR || retVal == "" || str == nullptr){
        delete[] str;
        return J;
    }
    vector<SimpleJson::Object> events;
    for(int i = 1; i < length; i++){
        Event event;
        event.Parse(str[i]);
        events.push_back(event.Format());
    }
    J.push_back({"events",events});

    delete[] str;
    return J;
}

int User::addEvent(std::string& value){
    NEDB _DB(USER_DIR + "/" + schoolid + "/" + classid + "/" + id);
    _DB.Mount("event");
    int res = _DB.Insert("event", "", value);
    _DB.Close();
    return res;
}

int User::delEvent(std::string& id){
    NEDB _DB(USER_DIR + "/" + schoolid + "/" + classid + "/" + id);
    _DB.Mount("event");
    int count;
    int res = _DB.Delete("event", "id=" + id, count);
    _DB.Close();
    return res;
}

int User::AddNew(std::string& detail){
    int errCode, len;
    errCode = __DATABASE.Insert("users", "", detail);
    if(errCode != NO_ERROR) return errCode;
    string* str = Split(detail, ',', len);
    if(len != 7) return PARAM_NUM_MISMATCH;
    this->id = str[0];
    this->auth = stoi(str[1]);
    this->name = str[2];
    this->gender = str[3];
    this->schoolid = str[4];
    this->majorid = str[5];
    this->classid = str[6];
    __DATABASE.Insert("token", "", id + "," + "123" + "," + str[1]);
    NEDB _DB(USER_DIR + "/" + schoolid + "/" + classid + "/" + id);
    errCode = _DB.DirInit();
    if(errCode != NO_ERROR) return errCode;
    errCode = _DB.Create("event", "id int,name text,start int64,end int64,loc int,info text");
    if(this->auth == USER_SCHOOL){
        _DB.Create("timetable", "id int,mon int,tue int,wed int,thur int,fri int");
    }
    _DB.Close();
    return errCode;
}
