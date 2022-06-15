#ifndef __USER_CONTROL_H__
#define __USER_CONTROL_H__
#include <common.h>
#include <interfaces.h>

#define USER_ADMIN 3
#define USER_SCHOOL 2
#define USER_CLASS 1
#define USER_COMMON 0

class User{
protected:
    std::string id;
    std::string name;
    int auth;
    std::string gender;
    std::string majorid;
    std::string majorName;
    std::string schoolid;
    std::string schoolName;
    std::string classid;
public:
    User(std::string id = "0");
    std::string getSchool(){return schoolid;}
    std::string getName(){return name;}
    int     Query(bool detail_name = true);
    Json    Format();
    int     Signin(std::string& passwd);
    int     Signup(std::string& passwd);
    int     Update(std::string& value);
    int     AddNew(std::string& detail);

    //Event Service
    Json    getEvents();
    int     addEvent(std::string& value);
    int     delEvent(std::string& id);

    //TimeTable Service
    Json    getTimeTable();
};

class Professor:public User{
public:
    Professor(std::string id = ""):User(id){};
    int AddHomework(std::string& detail);
    int DelHomework(std::string& detail);

};

class Class{
private:
    std::string id;
    std::string schoolid;
    int Query();
public:
    Class(std::string classid);
    int getMemberNum();
    Json getTimeTable(string schoolid="",string prof="");
    Json getList();
    int AddNew(std::string school);
    int AddCourse(std::string& courseid,std::string& prof,std::string& detail);

};


#endif
