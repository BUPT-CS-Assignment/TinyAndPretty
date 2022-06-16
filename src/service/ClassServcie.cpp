#include<service/Event.h>
#include <service/TAPSystem.h>
#include <service/User.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;


def_HttpEntry(API_Class, req)
{
    string userid(req.queryHeader("userid"));
    string token(req.queryHeader("token"));
    string function(req.queryHeader("function"));
    string body = req.getBody();

    CONSOLE_LOG(true,"api/class called [user:%s  function:%s]\n",userid.c_str(),function.c_str());

    if(function == "intro")
    {
        Class c(string(req.queryParam("classid")));
        int num = c.getMemberNum();
        return new HttpResponse{"班级人数: " + to_string(num),"NO_ERROR",HTTP_STATUS_200};
    }

    if(TokenCheck("10000", token) != TOKEN_ACCESS)
    {
        return new HttpResponse("ACCESS_DENIED", HTTP_STATUS_401);
    }

    if(function == "course")
    {
        Class c(string(req.queryParam("classid")));
        string course(req.queryParam("course"));
        string prof(req.queryParam("prof"));

        return new HttpResponse{
            "",NEexceptionName[c.AddCourse(course, prof, body)],HTTP_STATUS_200
        };
    }

    if(function == "new")
    {
        Class c(string(req.queryParam("classid")));

        return new HttpResponse{
            "",NEexceptionName[c.AddNew(string(req.queryParam("school")))],HTTP_STATUS_200
        };

    }

    if(function == "list")
    {
        Class c("0");
        Json J = c.getList();

        return new JsonResponse{
            J,"NO_ERROR",HTTP_STATUS_200
        };
    }

    if(function == "timetable")
    {
        Class c(string(req.queryParam("classid")));
        Json J = c.getTimeTable();

        return new JsonResponse{
            J,"NO_ERROR",HTTP_STATUS_200
        };
    }

    return new HttpResponse{"","NO_FUNCTION_MATCH",HTTP_STATUS_400};

}

def_HttpEntry(API_School, req)
{
    string userid(req.queryHeader("userid"));
    string token(req.queryHeader("token"));
    string function(req.queryHeader("function"));

    CONSOLE_LOG(true,"api/school called [user:%s  function:%s]\n",userid.c_str(),function.c_str());

    if(function == "intro")
    {
        string school(req.queryParam("school"));
        string filepath = SRC_DIR + "/school/" + school + "/intro.txt";
        return new FileResponse{
            filepath,"text/html","NO_ERROR"
        };
    }

    return new HttpResponse{"","NO_FUNCTION_MATCH",HTTP_STATUS_400};

}

def_HttpEntry(API_Major, req)
{
    string userid(req.queryHeader("userid"));
    string token(req.queryHeader("token"));
    string function(req.queryHeader("function"));

    CONSOLE_LOG(true,"api/major called [user:%s  function:%s]\n",userid.c_str(),function.c_str());

    if(function == "intro")
    {
        string major(req.queryParam("major"));
        string filepath = SRC_DIR + "/major/" + major + ".txt";
        return new FileResponse{
            filepath,"text/html","NO_ERROR"
        };
    }

    return new HttpResponse{"","NO_FUNCTION_MATCH",HTTP_STATUS_400};

}
