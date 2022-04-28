#ifndef __USER_CONTROL_H__
#define __USER_CONTROL_H__
#include <common.h>
#include <interfaces.h>

#define USER_ADMIN 1
#define USER_COMMON 0
#define USER_SIGN_OUT 0
#define USER_SignIn 1

class User;
extern BTREESTD::BalanceTree<User, int>COOKIE;

class User{
private:
    int id_;
    int auth_;
    int status_;
    NEDBSTD::NEDB* UserDB;
public:
    User();
    User(int id);
    User(int id, int auth);
    int SignIn(std::string passwdInput);
    int SignUp(std::string passwdInput);
    int SignOut();
    int getStatus(){
        return status_;
    }
    /////
    std::string Format();
    std::string GetInfo(std::string);
    int SetInfo(std::string);
};

#endif
