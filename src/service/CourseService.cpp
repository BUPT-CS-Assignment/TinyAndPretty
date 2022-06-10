#include<service/Event.h>
#include <service/TAPSystem.h>
#include <service/User.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;


def_HttpEntry(API_Timetable,req){
    std::string userid(req.queryHeader("userid"));
    std::string token(req.queryHeader("token"));
    std::string function(req.queryHeader("function"));
    std::string body = req.getBody();
    if(function != "fetch"){
        return new HttpResponse{"REQUEST_FUNCTION_UNKNOWN",HTTP_STATUS_400};
    }
    if(TokenCheck(userid,token) != TOKEN_ACCESS){
        return new HttpResponse("ACCESS_DENIED",HTTP_STATUS_401);
    }
    User user(userid);
    int errCode = user.Query();
    if( errCode != NO_ERROR){
        HttpResponse* HResp = new HttpResponse{""};
        HResp->appendHeader("msg",NEexceptionName[errCode]);
        return HResp;
    }
    Json j = user.getTimeTable();
    //CONSOLE_LOG(0,1,1,"User Info '%s'",j.stringize());
    JsonResponse *JResp = new JsonResponse{j};
    JResp->appendHeader("msg","NO_ERROR");
    return JResp;
    // }else if(function == "update"){
    //     int errCode = user.setInfo(body);
    //     HttpResponse* HResp = new HttpResponse{""};
    //     HResp->appendHeader("msg",NEexceptionName[errCode]);
    //     return HResp;
    // }
}