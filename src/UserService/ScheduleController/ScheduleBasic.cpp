#include <UserService/Preload.h>
#include <UserService/ScheduleControl.h>
#include <UserService/UserControl.h>
using namespace std;
using namespace NEDBSTD;

Course::Course(){
    id_ = 0;
    name_ = "NULL";
    prof_id_ = 0;
}

Course::Course(int id, string name, int prof_id){
    id_ = id;
    name_ = name;
    prof_id_ = prof_id;
}

Course::Course(int id){
    id_ = id;
    int count = 0;
    string retVal;
    int errCode = PRELOAD_DB.Select("CourseInfo", "Name,Prof", "ID=" + to_string(id_), count, retVal);
    if(errCode != NO_ERROR){
        name_ = "NULL";
        prof_id_ = 0;
    }
    else{
        int index1 = retVal.find(";");
        int index2 = retVal.find_last_of(",");
        name_ = retVal.substr(index1 + 1, index2 - index1 - 1);
        prof_id_ = stoi(retVal.substr(index2));
    }

}

string Course::GetName(){
    return name_;
}

std::string Course::GetProf(){
    User temp(prof_id_, USER_COMMON);
    return temp.GetInfo("Name");
}

int Course::AddCourse(){
    string sql = "insert into CourseInfo values (";
    int errCode = PRELOAD_DB.Insert("CourseInfo", "", to_string(id_) + "," + name_ + "," + to_string(prof_id_));
    return errCode;
}

int Course::DeleteCourse(){
    string sql = "delete from CourseInfo where ID = ";
    int count;
    int errCode = PRELOAD_DB.Delete("CourseInfo","ID="+to_string(id_),count);
    return errCode;
}