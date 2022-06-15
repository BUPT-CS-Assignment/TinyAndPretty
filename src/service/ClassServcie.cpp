#include<service/Event.h>
#include <service/TAPSystem.h>
#include <service/User.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;


def_HttpEntry(API_Class, req){
    string userid(req.queryHeader("userid"));
    string token(req.queryHeader("token"));
    string function(req.queryHeader("function"));
    string body = req.getBody();

    if(function == "intro"){
        Class c(string(req.queryParam("classid")));
        int num = c.getMemberNum();
        HttpResponse* HResp = new HttpResponse{"班级人数: "+to_string(num)};
        HResp->appendHeader("msg","NO_ERROR");
        return HResp;
    }

    if(TokenCheck("10000", token) != TOKEN_ACCESS){
        return new HttpResponse("ACCESS_DENIED", HTTP_STATUS_401);
    }

    if(function == "course"){
        Class c(string(req.queryParam("classid")));
        string course(req.queryParam("course"));
        string prof(req.queryParam("prof"));
        int errCode = c.AddCourse(course, prof, body);
        HttpResponse* HResp = new HttpResponse{""};
        HResp->appendHeader("msg", NEexceptionName[errCode]);
        return HResp;
    }
    else if(function == "new"){
        Class c(string(req.queryParam("classid")));
        int errCode = c.AddNew(string(req.queryParam("school")));
        HttpResponse* HResp = new HttpResponse{""};
        HResp->appendHeader("msg", NEexceptionName[errCode]);
        return HResp;
    }
    else if(function == "list"){
        Class c("0");
        Json J = c.getList();
        JsonResponse* JResp = new JsonResponse{J};
        JResp->appendHeader("msg", "NO_ERROR");
        return JResp;
    }
    else if(function == "timetable"){
        Class c(string(req.queryParam("classid")));
        Json J = c.getTimeTable();
        JsonResponse* JResp = new JsonResponse{J};
        JResp->appendHeader("msg", "NO_ERROR");
        return JResp;
    }
}

def_HttpEntry(API_School,req){
    string userid(req.queryHeader("userid"));
    string token(req.queryHeader("token"));
    string function(req.queryHeader("function"));

    if(function == "intro"){
        string school(req.queryParam("school"));
        string filepath = SRC_DIR + "/school/"+school+"/intro.txt";
        FileResponse* FResp = new FileResponse(filepath,"text/html");
        FResp->appendHeader("msg","NO_ERROR");
        return FResp;
    }


}

def_HttpEntry(API_Major,req){
    string userid(req.queryHeader("userid"));
    string token(req.queryHeader("token"));
    string function(req.queryHeader("function"));

    if(function == "intro"){
        string major(req.queryParam("major"));
        string filepath = SRC_DIR + "/major/"+major+".txt";
        FileResponse* FResp = new FileResponse(filepath,"text/html");
        FResp->appendHeader("msg","NO_ERROR");
        return FResp;
    }

}
