#include <service/User.h>
#include <service/TAPSystem.h>
#include <service/Event.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;

Class::Class(string classid){
    this->id = classid;
}

int Class::Query(){
    int count, errCode;
    string retVal;
    errCode = __DATABASE.Select("classes", "school", "id = " + id, count, retVal);
    if(count == 0) schoolid = "0";
    else    schoolid = retVal.substr(retVal.find(';') + 1);
    return errCode;
}

Json Class::getTimeTable(string schoolid, string prof){
    int count, len, length;
    string retVal;
    if(schoolid.length() == 0){
        Query();
        schoolid = this->schoolid;
    }
    NEDB _DB("/");
    if(prof.length() == 0){
        _DB.SetDir(USER_DIR + "/" + schoolid); _DB.Mount("timetable");
        _DB.Select("timetable", "*", "id > " + id + "000 and id < " + id + "999", count, retVal);
    }
    else{
        _DB.SetDir(USER_DIR + "/" + schoolid + "/0/" + prof);   _DB.Mount("timetable");
        _DB.Select("timetable", "*", "", count, retVal);
    }
    _DB.Close();

    Json J;
    if(count == 0) return J;
    vector<SimpleJson::Object> courses;

    string* str = Split(retVal, ';', len);
    for(int i = 1; i < len; i++){
        string* info = Split(str[i], ',', length);
        Course course(info[1]); course.Query(false);
        User user(info[2]); user.Query(false);
        vector<int> Tcode;
        for(int i = 3; i <= 7; i++){
            Tcode.push_back(stoi(info[i]));
        }
        __DATABASE.Select("landmark", "name", "id=" + info[8], count, retVal);
        string location = retVal.substr(retVal.find(';') + 1);
        int classid = stoll(info[0]) / 1000;
        courses.push_back({
            {"id",stoi(info[1])},
            {"class",classid},
            {"name",course.getName()},
            {"profid",stoi(info[2])},
            {"profname",user.getName()},
            {"wcode",course.getTime()},
            {"tcode",Tcode},
            {"location",location},
            {"room",info[9]},
            {"contact",info[10]}
            });
        delete[] info;
    }
    J.push_back({"courses",courses});
    delete[] str;
    return J;
}

Json Class::getList(){
    string ret; int count, len, length;
    int errCode = __DATABASE.Select("classes", "*", "", count, ret);
    Json J;
    if(errCode == NO_ERROR){
        ret = ret.substr(ret.find_first_of(";") + 1);
        ret = ret.substr(ret.find_first_of(";") + 1);
        string* str = Split(ret, ';', len);
        vector<SimpleJson::Object> js;
        for(int i = 0; i < len; i++){
            string* temp = Split(str[i], ',', length);
            js.push_back({
                {"id",stoi(temp[0])},
                {"school",stoi(temp[1])}
                });
            delete[] temp;
        }
        delete[] str;
        J.push_back({"data",js});
    }
    return J;
}

int Class::AddCourse(std::string& courseid, std::string& prof, std::string& detail){
    int errCode, count; string ret;
    if(__DATABASE.Select("courses", "id", "id=" + courseid, count, ret) != NO_ERROR){
        return DATA_NOT_FOUND;
    }
    Query();
    string dir = USER_DIR + "/" + schoolid;
    NEDB _DB(dir);
    errCode = _DB.Mount("timetable");
    if(errCode == FILE_NOT_FOUND){
        _DB.DirInit();
        _DB.SetDefaultPageSize(1000);
        _DB.Create("timetable", "id int64,course int,prof int,D1 int,D2 int,D3 int,D4 int,D5 int,loc int,room int,contact int64");
    }
    _DB.Close();

    string info(courseid + "," + prof + "," + detail);
    NEDB _DB2(dir); _DB2.Mount("timetable");
    errCode = _DB2.Select("timetable", "id", "id > " + id + "000 and id < " + id + "999", count, ret);
    if(count < 9) info = id + ("00" + to_string(count + 1)) + "," + info;
    else if(count < 99) info = id + ("0" + to_string(count + 1)) + "," + info;
    else if(count < 999) info = id + to_string(count) + "," + info;
    errCode = _DB2.Insert("timetable", "", info);
    _DB2.Close();

    User user(prof);
    user.Query();
    string dir2 = USER_DIR + "/" + user.getSchool() + "/0/" + prof;
    NEDB DB(dir2);
    errCode = DB.Mount("timetable");
    if(errCode == FILE_NOT_FOUND){
        DB.DirInit();
        DB.SetDefaultPageSize(800);
        DB.Create("timetable", "id int64,course int,prof int,D1 int,D2 int,D3 int,D4 int,D5 int,loc int,room int,contact int64");
    }
    DB.Close();

    NEDB DB2(dir2); DB2.Mount("timetable");
    errCode = DB2.Insert("timetable", "", info);

    if(errCode == NO_ERROR){
        DB2.SetDir(SRC_DIR + "/course/" + courseid + "/" + prof);
        DB2.DirInit();
    }
    DB2.Close();
    return errCode;
}


int Class::AddNew(string school){
    NEDB _DB(SRC_DIR + "/school/");
    _DB.Mount("classes");
    int errCode = _DB.Insert("classes", "", id + "," + school);
    if(errCode != NO_ERROR){
        return errCode;
    }
    _DB.SetDir(USER_DIR + "/" + school + "/" + id);
    errCode = _DB.DirInit();
    _DB.Close();
    return errCode;
}

int Class::getMemberNum(){
    NEDB DB(USER_DIR);

    if(DB.Mount("users") != NO_ERROR) return 0;
    int count; string ret;
    DB.Select("users", "id", "class = " + this->id, count, ret);
    return count;
}