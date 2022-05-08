#include<service/schedule/ScheduleControl.h>
#include <service/sys/ServerSys.h>
#include <service/user/UserControl.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;

def_HttpEntry(ScheduleIndex,req){
    std::string userid(req.queryHeader("Userid"));
    std::string token(req.queryHeader("Token"));
    std::string function(req.queryHeader("Function"));
    std::string body = req.getBody();
    CONSOLE_LOG(0,1,1,"UserPanel-Req [function='%s', userid='%s', token='%s']\n", function.c_str(),userid.c_str(),token.c_str());
    if(function == ""){
        return new FileResponse{"web/schedule/index.html","text/html"};
    }
    
    if(TokenCheck(userid,token) != TOKEN_ACCESS){
        return new HttpResponse("ACCESS_DENIED\r\n",HTTP_STATUS_401);
    }
    User user(userid);
    user.Init();
    Course course(userid);
    course.setClass(user.getClass());
    course.setSchool(user.getSchool());
    if(function == "fetch"){
        Json j = course.getTimeTable();
        //CONSOLE_LOG(0,1,1,"User Info '%s'",j.stringize());
        return new JsonResponse(j);
    }else if(function == "update"){
        int res = user.setInfo(body);
        return new HttpResponse{std::to_string(res)+"?"};
    }
    return new HttpResponse{"REQUEST_FUNCTION_UNKNOWN\r\n",HTTP_STATUS_400};

}