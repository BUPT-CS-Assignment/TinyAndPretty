#include <UserService/Preload.h>
#include <UserService/ScheduleControl.h>
#include <UserService/UserControl.h>
using namespace std;

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
    string sql = "select Name,Prof from CourseInfo where ID = " + to_string(id_) + ";";
    int res = MainDB.Exec(sql.c_str());
    if(res != 0){
        name_ = "NULL";
        prof_id_ = 0;
    }
    else{
        string val = MainDB.ReturnVal();
        int index1 = val.find(";");
        int index2 = val.find_last_of(",");
        name_ = val.substr(index1 + 1, index2 - index1 - 1);
        prof_id_ = stoi(val.substr(index2));
    }

}

string Course::GetName(){
    return name_;
}

std::string Course::GetProf(){
    User temp(prof_id_, USER_COMMON);
    return temp.GetName();
}

int Course::AddCourse(){
    string sql = "insert into CourseInfo values (";
    sql += (to_string(id_) + ",");
    sql += (name_ + ",");
    sql += (to_string(prof_id_) + ");");
    MainDB.Exec(sql.c_str());
    return MainDB.ErrCode();
}

int Course::DeleteCourse(){
    string sql = "delete from CourseInfo where ID = ";
    sql += (to_string(id_) + ";");
    MainDB.Exec(sql.c_str());
    return MainDB.ErrCode();
}