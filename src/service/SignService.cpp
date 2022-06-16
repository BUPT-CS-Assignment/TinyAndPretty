#include <service/User.h>
#include <service/TAPSystem.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;

def_HttpEntry(API_Access, req)
{
    string level(req.queryHeader("function"));
    string userid(req.queryHeader("userid"));
    string token(req.queryHeader("token"));
    if(TokenCheck((level == "3" ? "10000" : userid), token) != TOKEN_ACCESS)
    {
        return new HttpResponse("ACCESS_DENIED", HTTP_STATUS_401);
    }
    else
    {
        return new HttpResponse("NO_ERROR");
    }
}

def_HttpEntry(API_Signin, req)
{
    string userid(req.queryHeader("userid"));

    CONSOLE_LOG(true,"* api/signin called [user:%s]\n",userid.c_str());

    string passwd = req.getBody();
    User user(userid);
    int res = user.Signin(passwd);

    if(res == NO_ERROR)
    {
        CONSOLE_LOG(true,"user %s signed in\n",userid.c_str());
        HttpResponse* resp = new HttpResponse("NO_ERROR");
        resp->appendHeader("token", TokenSign(userid));
        return resp;
    }
    else if(res == -1)
    {
        return new HttpResponse{"PASSWORD_MISMATCH"};
    }
    else if(res == -2)
    {
        return new HttpResponse{"WAIT_FOR_CONFIRM"};
    }
    else if(res == 31)
    {
        return new HttpResponse{"USER_UNREGISTERED"};
    }
    else
    {
        return new HttpResponse{NEexceptionName[res]};
    }
}


def_HttpEntry(API_Signup, req)
{
    string userid(req.queryParam("userid"));

    CONSOLE_LOG(true,"* api/signup called [userid:%s]\n",userid.c_str());

    string passwd(req.getBody());
    User user(userid);

    return new HttpResponse{
        "",NEexceptionName[user.Signup(passwd)],HTTP_STATUS_200
    };
}