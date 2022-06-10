#include <service/Event.h>
#include <service/TAPSystem.h>
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

SimpleJson::Object Course::getCourseInfo(string courseid){
    int count;
    string retVal;
    __LSR__.Select("courses","*","id="+courseid,count,retVal);
    int length;
    string *str = Split(retVal,';',length);
    if(retVal == "" || count == 0 || str == nullptr){
        return SimpleJson::Object({});
    }
    //cout<<"retVal : "<<retVal<<endl;
    string *info = Split(str[1],',',length);
    __LSR__.Select("users","name","id="+info[3],count,retVal);
    string username = retVal.substr(retVal.find(';')+1);
    //loc
    __LSR__.Select("landmark","name","id="+info[4],count,retVal);
    string landmarkname = retVal.substr(retVal.find(';')+1);

    SimpleJson::Object J({
        {"id",info[0].c_str()},
        {"name",info[1].c_str()},
        {"time",stoi(info[2])},
        {"prof",username.c_str()},
        {"loc",landmarkname.c_str()},
        {"room",info[5].c_str()}
    });
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
    string * str = Split(retVal,';',length);
    //fields ; a ; b ;
    //fields: id,mon,tue,wed,thur,fri
    if(retVal == "" || str == nullptr){
        TempDB.Close();
        return J;
    }
    int length_temp;
    vector<SimpleJson::Object> courseInfo;
    vector<SimpleJson::Object> timeCode;
    for(int i = 1;i < length;i++){
        string * info = Split(str[i],',',length_temp);
        SimpleJson::Object obj = getCourseInfo(info[0]);
        int daycode[6];
        for(int j=1;j<=5;j++){
            daycode[j]=stoi(info[j]);
        }
        vector<int> code(daycode+1,daycode+6);
        timeCode.push_back(SimpleJson::Object({{"pos",i},{"timeCode",code}}));
        //tempJson.push_back({"daycode",code});
        courseInfo.push_back(obj);
        delete[] info;
    }
    J.push_back({"basic",timeCode});
    J.push_back({"detail",courseInfo});
    delete[] str;
    TempDB.Close();
    return J;
    /*
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
    */
}

/*

{"detail":
    [
        {
            "id":xx,...
        },
        {
            "id":xx,...
        }
    ]

}


*/
