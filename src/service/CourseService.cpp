#include<service/Event.h>
#include <service/TAPSystem.h>
#include <service/User.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;

def_HttpEntry(API_Course, req){
    string userid(req.queryHeader("userid"));
    string token(req.queryHeader("token"));
    string function(req.queryHeader("function"));
    string body = req.getBody();

    if(TokenCheck(userid, token) != TOKEN_ACCESS){
        return new HttpResponse("ACCESS_DENIED", HTTP_STATUS_401);
    }

    int errCode = COMMAND_UNDEFINED;
    if(function == "new"){
        if(userid != "10000")   return new HttpResponse("ACCESS_DENIED", HTTP_STATUS_401);
        Course course;
        int index = body.find_first_of(";");
        string str = body.substr(0, index);
        body = body.substr(index + 1);
        errCode = course.AddNew(str, body);

    }
    else if(function == "list"){
        if(userid != "10000") return new HttpResponse("ACCESS_DENIED\r\n", HTTP_STATUS_401);
        string ret; int count, len,length;
        int errCode = __DATABASE.Select("courses", "id,name", "", count, ret);
        Json J;
        if(errCode == NO_ERROR){
            ret = ret.substr(ret.find_first_of(";")+1);
            string* str = Split(ret,';',len);
            vector<SimpleJson::Object> js;
            for(int i = 0;i<len; i++){
                string* temp =Split(str[i],',',length);
                js.push_back({
                    {"id",stoi(temp[0])},
                    {"name",temp[1]}
                });
                delete [] temp;
            }
            delete [] str;
            J.push_back({"data",js});
        }
        JsonResponse* JResp = new JsonResponse{J};
        JResp->appendHeader("msg",NEexceptionName[errCode]);
        return JResp;
    }
    else if(function == "intro"){
        string id(req.queryParam("courseid"));
        string path = SRC_DIR + "/course/"+id+"/intro.txt";
        FileResponse* FResp = new FileResponse(path,"text/html");
        FResp->appendHeader("msg","NO_ERROR");
        return FResp;
    }
    else if(function == "addwork"){
        Course course(string(req.queryParam("courseid")));
        string classid(req.queryParam("classid"));
        errCode = course.AddWork(userid,classid,body);
        HttpResponse* HResp = new HttpResponse{""};
        HResp->appendHeader("msg",NEexceptionName[errCode]);
        return HResp;

    }else if(function == "getwork"){
        Course course(string(req.queryParam("courseid")));
        string classid(req.queryParam("classid"));
        string prof(req.queryParam("prof"));
        Json J = course.getWork(prof,classid);
        JsonResponse* JResp = new JsonResponse{J};
        JResp->appendHeader("msg","NO_ERROR");
        return JResp;

    }
    else if(function == "addexam"){
        Course course(string(req.queryParam("courseid")));
        errCode = course.AddExam(body);
    }else if(function == "getexam"){
        Course course(string(req.queryParam("courseid")));
        string school(req.queryParam("schoolid"));

    }
    else if(function == "files"){
        CONSOLE_LOG(0,1,1,"Get Files\n");
        Course course(string(req.queryParam("courseid")));
        string prof(req.queryParam("prof"));
        Json J = course.getFile(prof);
        JsonResponse* JResp = new JsonResponse{J};
        JResp->appendHeader("msg","NO_ERROR");        
        return JResp;
    }
    else if(function == "delete"){
        if(userid != "10000")   return new HttpResponse("ACCESS_DENIED", HTTP_STATUS_401);
        Course course(body);
        errCode = course.Remove();
    }


    HttpResponse* HResp = new HttpResponse{""};
    HResp->appendHeader("msg", NEexceptionName[errCode]);
    return HResp;
}


def_HttpEntry(API_Timetable, req){
    std::string userid(req.queryHeader("userid"));
    std::string token(req.queryHeader("token"));
    std::string function(req.queryHeader("function"));
    std::string body = req.getBody();
    std::string classid(req.queryParam("classid"));
    if(function != "fetch"){
        return new HttpResponse{"REQUEST_FUNCTION_UNKNOWN",HTTP_STATUS_400};
    }
    if(TokenCheck(userid, token) != TOKEN_ACCESS){
        return new HttpResponse("ACCESS_DENIED", HTTP_STATUS_401);
    }
    User user(userid);
    Json j;
    int errCode = user.Query();
    if(errCode != NO_ERROR){
        HttpResponse* HResp = new HttpResponse{""};
        HResp->appendHeader("msg", NEexceptionName[errCode]);
        return HResp;
    }
    j = user.getTimeTable();
    //CONSOLE_LOG(0,1,1,"User Info '%s'",j.stringize());
    JsonResponse* JResp = new JsonResponse{j};
    JResp->appendHeader("msg", "NO_ERROR");
    return JResp;
    // }else if(function == "update"){
    //     int errCode = user.setInfo(body);
    //     HttpResponse* HResp = new HttpResponse{""};
    //     HResp->appendHeader("msg",NEexceptionName[errCode]);
    //     return HResp;
    // }
}