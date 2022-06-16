#include <service/User.h>
#include <service/TAPSystem.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;

def_HttpEntry(API_User, req)
{
    string function(req.queryHeader("function"));
    string userid(req.queryHeader("userid"));
    string token(req.queryHeader("token"));

    CONSOLE_LOG(true,"api/user called [user:%s  function:%s]\n",userid.c_str(),function.c_str());

    string body(req.getBody());

    if(TokenCheck(userid, token) != TOKEN_ACCESS)
    {
        return new HttpResponse("ACCESS_DENIED\r\n", HTTP_STATUS_401);
    }

    if(function == "new")
    {
        if(userid != "10000"){
            return new HttpResponse("ACCESS_DENIED", HTTP_STATUS_401);
        }
        User user;
        return new HttpResponse{
            "",NEexceptionName[user.AddNew(body)],HTTP_STATUS_200
        };

    }

    if(function == "list")
    {
        if(userid != "10000")
        {
            return new HttpResponse("ACCESS_DENIED", HTTP_STATUS_401);
        }
        string ret;
        int count;
        int errCode = __DATABASE.Select("users", "id,auth,name", "", count, ret);
        Json J;
        if(errCode == NO_ERROR)
        {
            string str = ret.substr(ret.find_first_of(';') + 1);
            J.push_back({"list",str});
        }
        return new JsonResponse{
            J,NEexceptionName[errCode],HTTP_STATUS_200
        };
    }

    User user(userid);
    if(function == "fetch")
    {
        user.Query();
        Json J = user.Format();
        return new JsonResponse{
            J,"NO_ERROR",HTTP_STATUS_200
        };
    }
    
    if(function == "timetable")
    {
        user.Query();
        Json J = user.getTimeTable();
        return new JsonResponse{
            J,"NO_ERROR",HTTP_STATUS_200
        };
    }
    
    if(function == "pwd")
    {
        return new HttpResponse{
            "",NEexceptionName[user.ChangePwd(body)],HTTP_STATUS_200
        };
    }

    return new HttpResponse{"","NO_FUNCTION_MATCH",HTTP_STATUS_400};
}