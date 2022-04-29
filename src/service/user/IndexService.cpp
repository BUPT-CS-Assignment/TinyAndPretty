#include <service/user/UserControl.h>
#include <service/sys/ServerSys.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;

def_HttpEntry(UserIndex, req){
    std::string userid(req.queryHeader("Userid"));
    std::string token(req.queryHeader("Token"));
    std::string function(req.queryHeader("Function"));
    std::string body = req.getBody();
    CONSOLE_LOG(0,1,1,"UserPanel-Req [function='%s', userid='%s', token='%s']\n", function.c_str(),userid.c_str(),token.c_str());
    if(function == ""){
        return new FileResponse{"web/user/index.html","text/html"};
    }
    if(TokenCheck(userid,token) != TOKEN_ACCESS){
        return new HttpResponse("ACCESS_DENIED\r\n",HTTP_STATUS_401);
    }
    if(function == "Fetch"){
        std::string val = UserInfoFetch(userid);
        CONSOLE_LOG(0,1,1,"Info Fetch '%s'",val.c_str());
        return new HttpResponse{"0?"+val};
    }else if(function == "Update" && body != "__NULL__"){
        int res = UserInfoUpdate(userid,body);
        return new HttpResponse{std::to_string(res)+"?"};
    }
    return new HttpResponse{"REQUEST_FUNCTION_UNKNOWN\r\n",HTTP_STATUS_400};
    
    
}