#include <service/user/UserControl.h>
#include <service/sys/ServerSys.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;

def_HttpEntry(SignIn, req){
    string userid(req.queryHeader("Userid"));
    string passwd(req.queryHeader("Passwd"));
    string function(req.queryHeader("Function"));
    string body = req.getBody();
    CONSOLE_LOG(0,1,1,"Signin-Req [function='%s', userid='%s']\n", function.c_str(),userid.c_str());
    if(userid == "" || function == ""){
        return new FileResponse{"web/user/signin.html" , "text/html"};
    }
    if(function != "signin"){
        return new HttpResponse{"REQUEST_FUNCTION_UNKNOWN\r\n",HTTP_STATUS_400};
    }
    User user(userid);
    int res;
    if((res = user.Signin(passwd)) == 0){
        HttpResponse* resp = new HttpResponse("0?NO_ERROR");
        resp->appendHeader("Token",TokenSign(userid));
        return resp;
    }else if(res== -1){
        return new HttpResponse{"-1?PASSWORD_MISMATCH"};
    }else{
        return new HttpResponse{to_string(res)+"?"+NEexceptionName[res]};
    }
}


def_HttpEntry(SignUp, req){
    string userid(req.queryHeader("Userid"));
    string passwd(req.queryHeader("Passwd"));
    string function(req.queryHeader("Function"));
    CONSOLE_LOG(0,1,1,"SignUp-Req [function='%s', userid='%s']\n", function.c_str(),userid.c_str());
    if(userid == "" || function == ""){
        return new FileResponse{"web/user/signup.html" , "text/html"};
    }
    if(function != "signup"){
        return new HttpResponse{"REQUEST_FUNCTION_UNKNOWN\r\n",HTTP_STATUS_400};
    }
    User user(userid);
    int res = user.Signup(passwd);
    if(res == 0){
        return new HttpResponse("0?NO_ERROR");
    }else{
        return new HttpResponse{to_string(res)+"?"+NEexceptionName[res]};
    }
}