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
    std::string ans = req.getBody();
    if(ans == "__NULL__"){
        std::fstream fs("web/sql/terminal.html", std::ios::in | std::ios::binary);
        if(fs.is_open()) //std::cerr << "Return HTML\n";
            return new FileResponse{fs , "text/html"};
    }
    CONSOLE_LOG(0, 1, 1, "SQL-Request '%s'\n", ans.c_str());
    const char* sql = ans.c_str();
    /* DataBase Query */
    int count;
    string res;
    int errCode = __LSR__.Query(sql, count, res);
    CONSOLE_LOG(0, 1, 1, "Query OK Return Code %d\n", errCode);
    return new HttpResponse{std::to_string(errCode) + "?" + std::to_string(count) + "&" + res};
}

// EXAMPLE 2.2 FileResponse也支持从fstream发送文件，（目前仅存在与短连接）
def_HttpEntry(SqlHelp, req){
    std::fstream fs("web/sql/help.html", std::ios::in | std::ios::binary);
    if(fs.is_open()){
        return new FileResponse{fs , "text/html"};
    }
    else{
        return new HttpResponse{"404 NOT FOUND" , HTTP_STATUS_404};
    }
}


static int id;
def_HttpEntry(SqlTest, req){
    id++;
    int count;
    string res;
    CONSOLE_LOG(0, 1, 1, "Warning : DataBase Test %d Start\n", id);
    int errCode = __LSR__.Query("drop table database_test;", count, res);
    if(errCode != NO_ERROR){
        CONSOLE_LOG(0, 1, 1, "Warning : Query %d OK return Code %d\n", id, errCode);
        return new HttpResponse{std::to_string(errCode)};
    }
    errCode = __LSR__.Query("create table database_test(id int,name text);", count, res);
    if(errCode != NO_ERROR){
        CONSOLE_LOG(0, 1, 1, "Warning : Query %d OK return Code %d\n", id, errCode);
        return new HttpResponse{std::to_string(errCode)};
    }
    errCode = __LSR__.Query(DataBaseTestSQL.c_str(), count, res);
    CONSOLE_LOG(0, 1, 1, "Warning : Query %d OK return Code %d\n", id, errCode);
    return new HttpResponse{std::to_string(errCode)};

}