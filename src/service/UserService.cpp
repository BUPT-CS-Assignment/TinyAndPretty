#include <service/User.h>
#include <service/TAPSystem.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;

def_HttpEntry(API_User, req){
    string function(req.queryHeader("function"));
    string userid(req.queryHeader("userid"));
    string token(req.queryHeader("token"));
    string body(req.getBody());
    CONSOLE_LOG(0, 1, 1, "UserAPI Called [function='%s', userid='%s']\n", function.c_str(), userid.c_str());
    if(TokenCheck(userid, token) != TOKEN_ACCESS){
        return new HttpResponse("ACCESS_DENIED\r\n", HTTP_STATUS_401);
    }
    HttpResponse* HResp = new HttpResponse{""};

    if(function == "new"){
        if(userid != "10000") return new HttpResponse("ACCESS_DENIED\r\n", HTTP_STATUS_401);
        User user;
        int errCode = user.AddNew(body);
        HResp->appendHeader("msg",NEexceptionName[errCode]);
        return HResp;
    }else if(function == "alloc"){
        if(userid != "10000") return new HttpResponse("ACCESS_DENIED\r\n", HTTP_STATUS_401);
        Professor prof(string(req.queryParam("user")));
        int errCode = prof.Query(false);
        if(errCode != NO_ERROR){
            HResp->appendHeader("msg",NEexceptionName[errCode]);
            return HResp;
        }
        string course(req.queryParam("course"));
        errCode = prof.CourseAlloc(course,body);
        HResp->appendHeader("msg",NEexceptionName[errCode]);
            return HResp;
    }

    User user(userid);
    if(function == "fetch"){
        user.Query();
        Json j = user.Format();
        JsonResponse *JResp = new JsonResponse{j};
        JResp->appendHeader("msg","NO_ERROR");
        return JResp;
    }
    else if(function == "update"){
        std::string body = req.getBody();
        int errCode = user.Update(body);
        HResp->appendHeader("msg",NEexceptionName[errCode]);
        return HResp;
    }
    return new HttpResponse{"REQUEST_FUNCTION_UNKNOWN\r\n",HTTP_STATUS_400};
}