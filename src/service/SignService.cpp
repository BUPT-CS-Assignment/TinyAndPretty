#include <service/user/UserControl.h>
#include <service/sys/ServerSys.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;

def_HttpEntry(SignIn, req){
    string function(req.queryHeader("function"));
    if(function == ""){
        return new FileResponse{"web/signin.html" , "text/html"};
    }
    string userid(req.queryHeader("userid"));
    CONSOLE_LOG(0,1,1,"Signin-Req [function='%s', userid='%s']\n", function.c_str(),userid.c_str());
    if(function != "signin"){
        return new HttpResponse{"REQUEST_FUNCTION_UNKNOWN\r\n",HTTP_STATUS_400};
    }
    string passwd = req.getBody();
    User user(userid);
    int res;
    if((res = user.Signin(passwd)) == 0){
        HttpResponse* resp = new HttpResponse("NO_ERROR");
        resp->appendHeader("Token",TokenSign(userid));
        return resp;
    }else if(res== -1){
        return new HttpResponse{"PASSWORD_MISMATCH"};
    }else if(res== 31){
        return new HttpResponse{"USER_UNREGISTERED"};
    }else{
        return new HttpResponse{NEexceptionName[res]};
    }
}


def_HttpEntry(SignUp, req){
    string function(req.queryHeader("function"));
    if(function == ""){
        return new FileResponse{"web/signup.html" , "text/html"};
    }
    string userid(req.queryHeader("userid"));
    CONSOLE_LOG(0,1,1,"SignUp-Req [function='%s', userid='%s']\n", function.c_str(),userid.c_str());
    if(function != "signup"){
        return new HttpResponse{"REQUEST_FUNCTION_UNKNOWN\r\n",HTTP_STATUS_400};
    }
    string passwd = req.getBody();
    User user(userid);
    int res = user.Signup(passwd);
    if(res == 0){
        return new HttpResponse("NO_ERROR");
    }else{
        return new HttpResponse{NEexceptionName[res]};
    }
}