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
        HResp->appendHeader("msg", NEexceptionName[errCode]);
        return HResp;
    }
    else if(function == "list"){
        if(userid != "10000") return new HttpResponse("ACCESS_DENIED\r\n", HTTP_STATUS_401);
        string ret;int count,len;
        int errCode = __DATABASE.Select("users","id,auth,name","",count,ret);
        Json J;
        if(errCode == NO_ERROR){
            string str = ret.substr(ret.find_first_of(';')+1);
            J.push_back({"list",str});
        }
        JsonResponse* JResp = new JsonResponse{J};
        JResp->appendHeader("msg",NEexceptionName[errCode]);
        return JResp;
        return new JsonResponse{J};        
    }
    else if(function == "alloc"){
        if(userid != "10000") return new HttpResponse("ACCESS_DENIED\r\n", HTTP_STATUS_401);
        Professor prof(string(req.queryParam("user")));
        int errCode = prof.Query(false);
        if(errCode != NO_ERROR){
            HResp->appendHeader("msg", NEexceptionName[errCode]);
            return HResp;
        }
        string course(req.queryParam("course"));
        errCode = prof.CourseAlloc(course, body);
        HResp->appendHeader("msg", NEexceptionName[errCode]);
        return HResp;
    }

    User user(userid);
    if(function == "fetch"){
        user.Query();
        Json j = user.Format();
        JsonResponse* JResp = new JsonResponse{j};
        JResp->appendHeader("msg", "NO_ERROR");
        return JResp;
    }
    else if(function == "update"){
        std::string body = req.getBody();
        int errCode = user.Update(body);
        HResp->appendHeader("msg", NEexceptionName[errCode]);
        return HResp;
    }
    return new HttpResponse{"REQUEST_FUNCTION_UNKNOWN\r\n",HTTP_STATUS_400};
}