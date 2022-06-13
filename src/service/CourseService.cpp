#include<service/Event.h>
#include <service/TAPSystem.h>
#include <service/User.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;

def_HttpEntry(API_Course,req){
    string userid(req.queryHeader("userid"));
    string token(req.queryHeader("token"));
    string function(req.queryHeader("function"));
    string body = req.getBody();
    if(TokenCheck(userid,token) != TOKEN_ACCESS){
        return new HttpResponse("ACCESS_DENIED",HTTP_STATUS_401);
    }
    int errCode;
    if(function == "new"){
        if(userid != "10000")   return new HttpResponse("ACCESS_DENIED",HTTP_STATUS_401);
        Course course;
        int index = body.find_first_of(";");
        string str = body.substr(0,index);
        body = body.substr(index+1);
        errCode = course.AddNew(str,body);

    }else if(function == "delete"){
        if(userid != "10000")   return new HttpResponse("ACCESS_DENIED",HTTP_STATUS_401);
        Course course(body);
        errCode = course.Remove();
    }
    else if(function == "list"){
        if(userid != "10000") return new HttpResponse("ACCESS_DENIED\r\n", HTTP_STATUS_401);
        string ret;int count,len;
        int errCode = __DATABASE.Select("courses","id,name","",count,ret);
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
    else if(function == "addWork"){
        Course course(string(req.queryParam("id")));
        errCode = course.AddWork(body);

    }else if(function == "addExam"){
        Course course(string(req.queryParam("id")));
        errCode = course.AddExam(body);
    }

    HttpResponse* HResp = new HttpResponse{""};
    HResp->appendHeader("msg",NEexceptionName[errCode]);
    return HResp;
}


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