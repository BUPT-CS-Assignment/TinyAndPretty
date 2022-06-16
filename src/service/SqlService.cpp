#include <service/TAPSystem.h>
using namespace NEDBSTD;
using namespace UTILSTD;
using namespace std;

def_HttpEntry(API_SQL, req)
{
    string userid(req.queryHeader("userid"));
    string token(req.queryHeader("token"));
    string function(req.queryHeader("function"));
    string ans = req.getBody();

    if(TokenCheck("10000", token) != TOKEN_ACCESS)
    {
        return new HttpResponse("ACCESS_DENIED", HTTP_STATUS_401);
    }

    CONSOLE_LOG(true, "api/sql called [function:%s]\n", function.c_str());

    int count;
    string res;
    //Sql Query
    if(function == "run")
    {

        int errCode = __DATABASE.Query(ans, count, res);

        CONSOLE_LOG(true, "Query '%s' OK Return %s\n", ans.c_str(), NEexceptionName[errCode].c_str());

        Json J;
        J.push_back({"count",count});
        J.push_back({"retVal",res.c_str()});

        return new JsonResponse{
            J,NEexceptionName[errCode],HTTP_STATUS_200
        };
    }
    //Get Table List
    if(function == "list")
    {
        int errCode = __DATABASE.Query("select tables;", count, res);
        Json J;
        int length;
        string* str = Split(res, ',', length);
        vector<string> list;
        for(int i = 0; i < length; i++)
        {
            list.push_back(str[i]);
        }
        delete[] str;

        J.push_back({"list",list});

        CONSOLE_LOG(true, "Query OK Return %s\n", ans.c_str(), NEexceptionName[errCode].c_str());

        return new JsonResponse{
            J,NEexceptionName[errCode],HTTP_STATUS_200
        };
    }
    //Get Table Info
    if(function == "detail")
    {
        string tablename = ans;
        int errCode = __DATABASE.Select(tablename, "*", "", count, res);
        Json J;
        J.push_back({"name",tablename.c_str()});
        string fields = res.substr(0, res.find_first_of(";"));
        J.push_back({"field",fields.c_str()});
        string data = "";
        if(errCode == NO_ERROR)
        {
            data = res.substr(res.find_first_of(";") + 1);
        }
        J.push_back({"data",data.c_str()});

        CONSOLE_LOG(true, "Query OK Return %s\n", ans.c_str(), NEexceptionName[errCode].c_str());

        return new JsonResponse{
            J,NEexceptionName[errCode],HTTP_STATUS_200
        };
    }

    int errCode;
    string* str = Split(ans, ';', count);

    //Update Table Info
    if(function == "update")
    {
        errCode = __DATABASE.Update(str[0], str[2], str[1], count);
    }

    //insert Table Info
    else if(function == "insert")
    {
        errCode = __DATABASE.Insert(str[0], str[1], str[2]);
    }

    //delete value
    else if(function == "delete")
    {
        errCode = __DATABASE.Delete(str[0], str[1], count);
    }

    else if(function == "drop")
    {
        errCode = __DATABASE.Drop(ans);
    }

    else
    {
        return new HttpResponse{"","NO_FUNCTION_MATCH",HTTP_STATUS_400};
    }
    delete[] str;

    CONSOLE_LOG(true, "Query OK Return %s\n", ans.c_str(), NEexceptionName[errCode].c_str());

    return new HttpResponse{
        "",NEexceptionName[errCode],HTTP_STATUS_200
    };

}

// EXAMPLE 2.2 FileResponse也支持从fstream发送文件，（目前仅存在与短连接）
def_HttpEntry(API_SQL_Help, req)
{
    return new FileResponse{"web/sql/help.html" , "text/html"};
}

std::string DataBaseTestSQL = "";
void SQLTestGenerate()
{
    for(int i = 1; i <= 100; i++)
    {
        DataBaseTestSQL += "insert into database_test values (" + std::to_string(i) + ",0xff);";
    }
    DataBaseTestSQL += "delete from database_test where id>40 and id<160;select * from database_test;";
}

static int id;
def_HttpEntry(API_SQL_Test, req)
{

    CONSOLE_LOG(true, "api/sql/test called\n");

    id++;
    int count;
    string res;
    CONSOLE_LOG(true, "TEST ID %d\n", id);

    int errCode = __DATABASE.Query("drop table database_test;", count, res);
    CONSOLE_LOG(true, "Dropping: Query %d OK return Code %d\n", id, errCode);
    
    errCode = __DATABASE.Query("create table database_test(id int,name text);", count, res);
    CONSOLE_LOG(true, "Creating: Query %d OK return Code %d\n", id, errCode);

    errCode = __DATABASE.Query(DataBaseTestSQL.c_str(), count, res);
    CONSOLE_LOG(true, "Testing: Query %d OK return Code %d\n", id, errCode);
    
    return new HttpResponse{NEexceptionName[errCode]};

}
