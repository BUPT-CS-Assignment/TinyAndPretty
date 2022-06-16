#include<service/Event.h>
#include <service/TAPSystem.h>
#include <service/User.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;

def_HttpEntry(API_Event, req)
{
    std::string userid(req.queryHeader("userid"));
    std::string token(req.queryHeader("token"));
    std::string function(req.queryHeader("function"));

    CONSOLE_LOG(true,"* api/event called [user:%s  function:%s]\n",userid.c_str(),function.c_str());

    std::string body = req.getBody();


    if(TokenCheck(userid, token) != TOKEN_ACCESS)
    {
        return new HttpResponse("ACCESS_DENIED", HTTP_STATUS_401);
    }

    User user(userid);
    int errCode = user.Query();

    if(errCode != NO_ERROR)
    {
        return new HttpResponse{
            "",NEexceptionName[errCode],HTTP_STATUS_200
        };
    }

    if(function == "fetch")
    {
        Json J = user.getEvents();
        return new JsonResponse{
            J,"NO_ERROR",HTTP_STATUS_200
        };
    }

    if(function == "new")
    {
        return new HttpResponse{
            "",NEexceptionName[user.addEvent(body)],HTTP_STATUS_200
        };
    }

    return new HttpResponse{"","NO_FUNCTION_MATCH",HTTP_STATUS_400};
}
