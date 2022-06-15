#include <service/User.h>
#include <service/TAPSystem.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;

def_HttpEntry(API_Access, req){
    string level(req.queryHeader("function"));
    string userid(req.queryHeader("userid"));
    string token(req.queryHeader("token"));
    if(TokenCheck((level == "3" ? "10000" : userid), token) != TOKEN_ACCESS){
        return new HttpResponse("ACCESS_DENIED", HTTP_STATUS_401);
    }
    else{
        return new HttpResponse("NO_ERROR");
    }
}

def_HttpEntry(API_Signin, req){
    string function(req.queryHeader("function"));
    string userid(req.queryHeader("userid"));
    CONSOLE_LOG(0, 1, 1, "Signin-Req [function='%s', userid='%s']\n", function.c_str(), userid.c_str());
    if(function != "signin"){
        return new HttpResponse{"REQUEST_FUNCTION_UNKNOWN\r\n",HTTP_STATUS_400};
    }
    string passwd = req.getBody();
    User user(userid);
    int res;
    if((res = user.Signin(passwd)) == 0){
        HttpResponse* resp = new HttpResponse("NO_ERROR");
        resp->appendHeader("token", TokenSign(userid));
        return resp;
    }
    else if(res == -1){
        return new HttpResponse{"PASSWORD_MISMATCH"};
    }
    else if(res == -2){
        return new HttpResponse{"WAIT_FOR_CONFIRM"};
    }
    else if(res == 31){
        return new HttpResponse{"USER_UNREGISTERED"};
    }
    else{
        return new HttpResponse{NEexceptionName[res]};
    }
}


def_HttpEntry(API_Signup, req){

    string userid(req.queryParam("userid"));
    CONSOLE_LOG(0, 1, 1, "API_Signup-Req [userid='%s']\n", userid.c_str());

    string passwd(req.getBody());
    User user(userid);
    int res = user.Signup(passwd);
    HttpResponse* HResp = new HttpResponse{""};
    HResp->appendHeader("msg",NEexceptionName[res]);
    return HResp;
}