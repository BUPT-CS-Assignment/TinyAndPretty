#ifndef __USER_CONTROL_H__
#define __USER_CONTROL_H__
#include <common.h>
#include <interfaces.h>

#define USER_ADMIN 0
#define USER_SCHOOL 1
#define USER_CLASS 2
#define USER_COMMON 3

class User{
private:
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
    User(std::string id);
    int     Query();
    Json    Format();
    int     Signin(std::string& passwd);
    int     Signup(std::string& passwd);
    int     Update(std::string& value);
    Json    getEvents();
    int     addEvent(std::string& value);
    int     delEvent(std::string& id);
    Json    getTimeTable();

};

#endif
