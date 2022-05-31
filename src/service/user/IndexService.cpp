#include <service/user/UserControl.h>
#include <service/sys/ServerSys.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;

def_HttpEntry(UserIndex, req){
    string function(req.queryHeader("function"));
    if(function == ""){
        return new FileResponse{"web/user/index.html" , "text/html"};
    }
    string userid(req.queryHeader("userid"));
    std::string token(req.queryHeader("token"));
    CONSOLE_LOG(0, 1, 1, "UserPanel-Req [function='%s', userid='%s', token='%s']\n", function.c_str(), userid.c_str(), token.c_str());
    if(TokenCheck(userid, token) != TOKEN_ACCESS){
        //return new HttpResponse("ACCESS_DENIED\r\n", HTTP_STATUS_401);
    }
    User user(userid);
    user.Init();
    if(function != "fetch" && function != "update"){
        return new HttpResponse{"REQUEST_FUNCTION_UNKNOWN\r\n",HTTP_STATUS_400};
    }
    if(function == "fetch"){
        Json j = user.getInfo();
        //CONSOLE_LOG(0,1,1,"User Info '%s'",j.stringize());
        JsonResponse *JResp = new JsonResponse{j};
        JResp->appendHeader("msg","NO_ERROR");
        return JResp;
    }
    else if(function == "update"){
        std::string body = req.getBody();
        int errCode = user.setInfo(body);
        HttpResponse* HResp = new HttpResponse{""};
        HResp->appendHeader("msg",NEexceptionName[errCode]);
        return HResp;
    }
}