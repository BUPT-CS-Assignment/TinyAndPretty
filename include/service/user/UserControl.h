#ifndef __USER_CONTROL_H__
#define __USER_CONTROL_H__
#include <common.h>
#include <interfaces.h>

#define USER_ADMIN 0
#define USER_SCHOOL 1
#define USER_CLASS 2
#define USER_PERSON 3

class User{
private:
    std::string id;
    std::string name;
    int auth;
    std::string gender;
    std::string majorid;
    std::string schoolid;
    std::string classid;
public:
    User(std::string id);
    void Init();
    std::string getID();
    int getAuth();
    std::string getGender();
    std::string getName();
    std::string getSchool();
    std::string getMajor();
    std::string getClass();
    void setAuth(int auth);
    int Signin(std::string& passwd);
    int Signup(std::string& passwd);
    Json getInfo();
    int setInfo(std::string& value);

};



#endif
