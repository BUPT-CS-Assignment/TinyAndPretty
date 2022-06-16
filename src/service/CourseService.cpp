#include<service/Event.h>
#include <service/TAPSystem.h>
#include <service/User.h>
using namespace std;
using namespace NEDBSTD;
using namespace UTILSTD;

def_HttpEntry(API_Course, req)
{
    string userid(req.queryHeader("userid"));
    string token(req.queryHeader("token"));
    string function(req.queryHeader("function"));

    CONSOLE_LOG(true,"api/course called [user:%s  function:%s]\n",userid.c_str(),function.c_str());

    string body = req.getBody();

    if(TokenCheck(userid, token) != TOKEN_ACCESS)
    {
        return new HttpResponse("ACCESS_DENIED", HTTP_STATUS_401);
    }

    if(function == "new")
    {
        if(userid != "10000")
        {
            return new HttpResponse("ACCESS_DENIED", HTTP_STATUS_401);
        }
        Course course;
        int index = body.find_first_of(";");
        string str = body.substr(0, index);
        body = body.substr(index + 1);

        return new HttpResponse{
            "",NEexceptionName[course.AddNew(str, body)],HTTP_STATUS_200
        };

    }

    if(function == "list")
    {
        if(userid != "10000")
        {
            return new HttpResponse("ACCESS_DENIED\r\n", HTTP_STATUS_401);
        }

        Course c;
        Json J = c.getAll();
        return new JsonResponse{
            J,"NO_ERROR",HTTP_STATUS_200
        };
    }

    if(function == "intro")
    {
        string id(req.queryParam("courseid"));
        string path = SRC_DIR + "/course/" + id + "/intro.txt";

        return new FileResponse{
            path,"text/html","NO_ERROR"
        };
    }

    if(function == "addwork")
    {
        Course course(string(req.queryParam("courseid")));
        string classid(req.queryParam("classid"));

        return new HttpResponse{
            "",NEexceptionName[course.AddWork(userid, classid, body)],HTTP_STATUS_200
        };
    }

    if(function == "getwork")
    {
        Course course(string(req.queryParam("courseid")));
        string classid(req.queryParam("classid"));
        string prof(req.queryParam("prof"));
        Json J = course.getWork(prof, classid);

        return new JsonResponse{
            J,"NO_ERROR",HTTP_STATUS_200
        };

    }

    if(function == "addexam")
    {
        Course course(string(req.queryParam("courseid")));

        return new HttpResponse{
            "",NEexceptionName[course.AddExam(body)],HTTP_STATUS_200
        };

    }

    if(function == "getexam")
    {
        Course course(string(req.queryParam("courseid")));
        Json J;
        if(userid == "10000")
        {
            J = course.getExam(userid, true);
        }
        else
        {
            string classid(req.queryParam("classid"));
            string schoolid;
            if(classid != "0")
            {
                Class c(classid); c.Query();
                schoolid = c.getSchool();
            }
            else
            {
                schoolid = string(req.queryParam("schoolid"));
            }
            J = course.getExam(schoolid);
        }
        return new JsonResponse{
            J,"NO_ERROR",HTTP_STATUS_200
        };
    }

    if(function == "files")
    {
        Course course(string(req.queryParam("courseid")));
        string prof(req.queryParam("prof"));
        Json J = course.getFile(prof);

        return new JsonResponse{
            J,"NO_ERROR",HTTP_STATUS_200
        };
    }

    return new HttpResponse{"","NO_FUNCTION_MATCH",HTTP_STATUS_400};

}


def_HttpEntry(API_Timetable, req)
{
    std::string userid(req.queryHeader("userid"));
    std::string token(req.queryHeader("token"));
    std::string function(req.queryHeader("function"));

    CONSOLE_LOG(true,"api/timetable called [user:%s  function:%s]\n",userid.c_str(),function.c_str());

    std::string body = req.getBody();
    std::string classid(req.queryParam("classid"));

    if(TokenCheck(userid, token) != TOKEN_ACCESS)
    {
        return new HttpResponse("ACCESS_DENIED", HTTP_STATUS_401);
    }

    if(function == "fetch"){
        User user(userid);
        Json J;
        int errCode = user.Query();
        if(errCode != NO_ERROR)
        {
            return new HttpResponse{
                "",NEexceptionName[errCode],HTTP_STATUS_204
            };
        }

        J = user.getTimeTable();

        return new JsonResponse{
            J,"NO_ERROR",HTTP_STATUS_200
        };
    }
    
    return new HttpResponse{"","NO_FUNCTION_MATCH",HTTP_STATUS_400};
}