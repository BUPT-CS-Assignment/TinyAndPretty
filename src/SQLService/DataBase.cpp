#include <UserService/Preload.h>

std::string DataBaseTestSQL = "";
void SQLTestGenerate(){
    
    for(int i = 1; i <= 100; i++){
        DataBaseTestSQL += "insert into database_test values (" + std::to_string(i) + ",0xff);";
    }
    DataBaseTestSQL += "delete from database_test where id>40 and id<160;select * from database_test;";
}

def_HttpEntry(SQL_RUN, req){
    int errCode = 0;
    std::string val = "";
    int count = 0;
    std::string ans = req.getBody();
    if(ans == "__NULL__"){
        std::fstream fs("web/sql/DataBase.html", std::ios::in | std::ios::binary);
        if(fs.is_open()) //std::cerr << "Return HTML\n";
            return new FileResponse{fs , "text/html"};
    }
    CONSOLE_LOG(0, "SQL-Request '%s'\n", ans.c_str());
    const char* sql = ans.c_str();
    /* DataBase Query */
    MainDB.Exec(sql);
    errCode = MainDB.ErrCode();    //获取错误码
    val = MainDB.ReturnVal();      //获取查询信息
    count = MainDB.Count();        //获取查询数
    CONSOLE_LOG(0, "Query OK return Code %d\n", errCode);
    return new HttpResponse{std::to_string(errCode) + "?" + std::to_string(count) + "&" + val};
}

// EXAMPLE 2.2 FileResponse也支持从fstream发送文件，（目前仅存在与短连接）
def_HttpEntry(SQL_HELP, req){
    std::fstream fs("web/sql/SqlHelp.html", std::ios::in | std::ios::binary);
    if(fs.is_open()){
        return new FileResponse{fs , "text/html"};
    }
    else{
        return new HttpResponse{"404\nNOT FOUND" , HTTP_STATUS_404};
    }
}


static int id;
def_HttpEntry(SQL_TEST, req){
    id++;
    CONSOLE_LOG(0, "Warning : DataBase Test %d Start\n", id);
    MainDB.Exec("drop table database_test;");
    MainDB.Exec("create table database_test(id int,name text);");
    int errcode = MainDB.Exec(DataBaseTestSQL.c_str());
    CONSOLE_LOG(0, "Warning : Query %d OK return Code %d\n", id, errcode);
    return new HttpResponse{std::to_string(errcode)};

}