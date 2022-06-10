#include<service/Event.h>
#include <service/TAPSystem.h>
#include <service/User.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;

def_HttpEntry(API_Event,req){
    std::string userid(req.queryHeader("userid"));
    std::string token(req.queryHeader("token"));
    std::string function(req.queryHeader("function"));
    std::string body = req.getBody();
    CONSOLE_LOG(0,1,1,"Sche-Req [function='%s', userid='%s', token='%s']\n", function.c_str(),userid.c_str(),token.c_str());
    // if(function != "fetch" && function != "add" && function != "update" &&){
    //     return new HttpResponse{"REQUEST_FUNCTION_UNKNOWN",HTTP_STATUS_400};
    // }
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
    if(function == "fetch"){
        Json j = user.getEvents();
        //CONSOLE_LOG(0,1,1,"User Info '%s'",j.stringize());
        JsonResponse *JResp = new JsonResponse{j};
        JResp->appendHeader("msg","NO_ERROR");
        return JResp;
    }else if(function == "delete"){
        
    }
    
    
}
