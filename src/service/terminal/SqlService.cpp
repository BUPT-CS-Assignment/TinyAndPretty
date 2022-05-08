#include <service/sys/ServerSys.h>
using namespace NEDBSTD;
using namespace UTILSTD;
using namespace std;

std::string DataBaseTestSQL = "";
void SQLTestGenerate(){

    for(int i = 1; i <= 100; i++){
        DataBaseTestSQL += "insert into database_test values (" + std::to_string(i) + ",0xff);";
    }
    DataBaseTestSQL += "delete from database_test where id>40 and id<160;select * from database_test;";
}

def_HttpEntry(SqlRun, req){
    string userid(req.queryHeader("Userid"));
    string token(req.queryHeader("Token"));
    string function(req.queryHeader("Function"));
    string ans = req.getBody();
    if(function == ""){
        return new FileResponse{"web/sql/terminal.html" , "text/html"};
    }
    if(TokenCheck("10000",token) != TOKEN_ACCESS){
        return new HttpResponse("ACCESS_DENIED\r\n",HTTP_STATUS_401);
    }
    if(function == "authenticate"){
        return new HttpResponse{""};
    }
    int count;
    string res;
    //Sql Query
    if(function == "run"){
        CONSOLE_LOG(0, 1, 1, "SQL-Request '%s'\n", ans.c_str());
        const char* sql = ans.c_str();
        int errCode = __LSR__.Query(sql, count, res);
        CONSOLE_LOG(0, 1, 1, "Query OK Return Code %d\n", errCode);
        Json J;
        J.push_back({"msg",NEexceptionName[errCode].c_str()});
        J.push_back({"count",count});
        J.push_back({"retVal",res.c_str()});
        return new JsonResponse{J};
        //return new HttpResponse{to_string(errCode) + "?" + std::to_string(count) + "&" + res};
    }
    //Get Table List
    if(function == "list"){
        CONSOLE_LOG(0, 1, 1, "Table List Req\n");
        int errCode = __LSR__.Query("select tables;",count,res);
        CONSOLE_LOG(0, 1, 1, "Query OK Return Code %d\n", errCode);
        Json J;
        int length;
        string* str = Split(res,',',length);
        vector<string> list;
        for(int i = 0; i < length; i++){
            list.push_back(str[i]);
        }
        delete[] str;
        J.push_back({"msg",NEexceptionName[errCode].c_str()});
        J.push_back({"list",list});
        return new JsonResponse{J};        
        //return new HttpResponse{to_string(errCode) + "?" + std::to_string(count) + "&" + res};
    }
    //Get Table Info
    if(function == "detail"){
        string tablename(req.queryParam("table"));
        int errCode = __LSR__.Select(tablename,"*","",count,res);
        CONSOLE_LOG(0, 1, 1, "Query OK Return Code %d\n", errCode);
        Json J;
        J.push_back({"name",tablename.c_str()});
        if(errCode!=NO_ERROR) return new JsonResponse{J};
        string fields = res.substr(0,res.find_first_of(";"));
        string data = res.substr(res.find_first_of(";")+1);
        J.push_back({"field",fields.c_str()});
        J.push_back({"data",data.c_str()});
        return new JsonResponse{J};
        //return new HttpResponse{to_string(errCode) + "?" + std::to_string(count) + "&" + res};
    }
    //Update Table Info
    if(function == "update"){
        string * str = Split(ans,';',count);
        int errCode = __LSR__.Update(str[0],str[1],str[2],count);
        CONSOLE_LOG(0, 1, 1, "Query OK Return Code %d\n", errCode);
    }
    return new HttpResponse{"REQUEST_FUNCTION_UNKNOWN\r\n",HTTP_STATUS_400};
}

// EXAMPLE 2.2 FileResponse也支持从fstream发送文件，（目前仅存在与短连接）
def_HttpEntry(SqlHelp, req){
    return new FileResponse{"web/sql/help.html" , "text/html"};
}

static int id;
def_HttpEntry(SqlTest, req){
    id++;
    int count;
    string res;
    CONSOLE_LOG(0, 1, 1, "Warning : DataBase Test %d Start\n", id);
    int errCode = __LSR__.Query("drop table database_test;", count, res);
    CONSOLE_LOG(0, 1, 1, "Dropping table... Warning : Query %d OK return Code %d\n", id, errCode);
    errCode = __LSR__.Query("create table database_test(id int,name text);", count, res);
    CONSOLE_LOG(0, 1, 1, "Creating table... Warning : Query %d OK return Code %d\n", id, errCode);
    errCode = __LSR__.Query(DataBaseTestSQL.c_str(), count, res);
    CONSOLE_LOG(0, 1, 1, "Testing... Warning : Query %d OK return Code %d\n", id, errCode);
    return new HttpResponse{std::to_string(errCode)};

}
