#ifndef __USER_CONTROL_H__
#define __USER_CONTROL_H__
#include <common.h>
#include <interfaces.h>

#define USER_ADMIN 1
#define USER_COMMON 0
#define USER_SIGN_OUT 0
#define USER_SignIn 1

int UserSignIn(std::string userid,std::string passwdInput);
int UserSignUp(std::string userid,std::string passwdInput);
std::string UserInfoFetch(std::string userid);
int UserInfoUpdate(std::string userid,std::string value);


#endif
