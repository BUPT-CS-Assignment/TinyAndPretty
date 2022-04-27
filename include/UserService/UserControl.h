#ifndef __USER_CONTROL_H__
#define __USER_CONTROL_H__
#include <common.h>
#include <interfaces.h>

#define USER_ADMIN 1
#define USER_COMMON 0
#define USER_SIGN_OUT 0
#define USER_SIGN_IN 1

class User{
private:
    int id_;
    int auth_;
    int status_;
public:
    User();
    User(int id,int auth);
    int SignIn(std::string passwdInput);
    int SignUp(std::string passwdInput);
    int SignOut();
    int getStatus(){
        return status_;
    }

};

extern BalanceTree<User,int> UserStatus;

#endif
