#include<service/schedule/ScheduleControl.h>
#include <service/sys/ServerSys.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;

Course::Course(string id){
    schoolid = "0";
    classid = "0";
    userid = id;
}

void Course::setClass(std::string id){
    classid = id;
}

void Course::setSchool(std::string id){
    schoolid = id;
}

std::string Course::getClass(){
    return classid;
}

std::string Course::getSchool(){
    return schoolid;
}

std::string Course::getUser(){
    return userid;
}

Json Course::getCourseInfo(string courseid){
    int count;
    string retVal;
    __LSR__.Select("courses","*","id="+courseid,count,retVal);
    int length;
    Json J;
    string *str = Split(retVal,';',length);
    if(retVal == "" || count == 0 || str == nullptr){
        return J;
    }
    cout<<"retVal : "<<retVal<<endl;
    string *info = Split(str[1],',',length);
    //name ,id
    J.push_back({"id",info[0].c_str()});
    J.push_back({"name",info[1].c_str()});
    //time
    J.push_back({"time",stoi(info[2])});
    //prof
    __LSR__.Select("users","name","id="+info[3],count,retVal);
    J.push_back({"prof",retVal.substr(retVal.find(';')+1).c_str()});
    //loc
    __LSR__.Select("landmark","name","id="+info[4],count,retVal);
    J.push_back({"loc",retVal.substr(retVal.find(';')+1).c_str()});
    //room
    J.push_back({"room",info[5].c_str()});
    delete[] str;
    delete[] info;
    return J;
}

Json Course::getTimeTable(){
    int count;
    string retVal;
    NEDB TempDB(USER_DIR + "/" + schoolid + "/" + classid);
    TempDB.Mount("timetable");
    int errCode = TempDB.Select("timetable","*","",count,retVal);
    int length;
    Json J;
    string * str = Split(retVal,';',length); //fields ; a ; b ;
    if(retVal == "" || str == nullptr){
        TempDB.Close();
        return J;
    }
    int templ;
    string * field = Split(str[0],',',templ);
    for(int i = 1; i < length; i++){
        int l;
        string * classes = Split(str[i],',',l);
        cout<<"Push Back Success 0"<<endl;
        for(int j = 1; j < l; j++){
            string classinfo = "";
            if(classes[j]!="0"){
                classinfo = getCourseInfo(classes[j]).stringize();
            }
            CONSOLE_LOG(0,1,1,"Class Info '%s'\n",classinfo.c_str());
            J.push_back({field[j].c_str(),classinfo.c_str()});
            cout<<"Push Back Success 1"<<endl;
        }
        delete[] classes;
    }
    TempDB.Close();
    delete[] str;
    delete[] field;
    return J;
}


