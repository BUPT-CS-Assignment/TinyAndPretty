#include <test/define.h>
// EXAMPLE 0 
// 如果需要添加一个入口， 需要在三处文件修改 （以后会简化吧...）：
// 1. interfaces.h
// 2. router.conf
// 3. test.cpp (your own .cpp)

void stringConcat(std::string& tar){;}  //recursion ending
template<typename T , typename... Args>
void stringConcat(std::string& tar , T value , Args... args){
	tar += std::string{value};
	stringConcat(tar , args...);
}

// EXAMPLE 1.0 HttpRequest 的一些基本使用,默认以text/html，参数为std::string
def_HttpEntry(Link_Start , request){
    std::string t {};

    stringConcat( t ,
    "<body><h1>Hello ! Dear Friend ,</h1>" ,
    "<h3>I know you comes from ...",request.queryClientIP(),"!</h3> " // 查询客户IP
    "<h3>And also you comes by ...",request.HttpVer()," <-With-> ",request.Method(),"!</h3> ", // 查询http版本和请求方法
    "<h3>What's more, you have ...",request.queryHeader("Content-Length"),"bytes !</h3> ");   // 根据key查询http header；

    stringConcat(t , 
    "<h1>Wow ! It seems that you Bring me a PRESENT</h1>",
    "<h3> There are",request.Length(),"gifts for me ,thank you! : ) </h3> " ,
    "<h3>Let me see... </h3>");
    try {   // multipart/from-data的使用 ： 根据key查找，支持fstream和string
        std::string ans = request.queryForm("Key");
        t += ans;
        ans = request.queryForm("Action");
        t += ans;
    } catch (const HttpException& e) {
        switch (e)
        {
        case HttpException::NON_POS : //NON_POS : 有form，但没有这个key
            t += "OMG , I FAIL!";
            break;
        case HttpException::NON_FORM: // NON_FORM : 没有form
            t += "<h1>NOTHING!</h1>";
            t += "<h3>no laughing matter... </h3>";
            break;
        default: break;
        }
    }

    stringConcat(t , 
    "<h1>Look up! There are patameters following you!</h1>",
    "<h3> It is : id = " ,request.queryParam("id") , " </h3> " ,// 查询url参数。若没有为""(空串)
    "<a herf=\"https://github.com/NoUITeam/TinyAndPretty\">Click ME for more info! </a></body>");
    return new HttpResponse{t};
}


// EXAMPLE 2.1 FileResponse支持直接从指定文件路径发送文件，效率比较高，但是仅为只读文件
def_HttpEntry(Lent_Book , req){
    return new FileResponse{"utils/out" , "application/pdf"};
}

// EXAMPLE 2.2 FileResponse也支持从fstream发送文件，（目前仅存在与短连接）
def_HttpEntry(SQL_HELP , req){
    std::fstream fs("web/sqlService/sql_help.html", std::ios::in | std::ios::binary);
    if(fs.is_open()){
        //std::cerr << "SQl HELP\n";
        return new FileResponse{fs , "text/html"};
    }else{
        return new HttpResponse{"404\nNOT FOUND" , HTTP_STATUS_404};
    }
}
// EXAMPLE 2.3 FileResponse建议使用c++17的std::filesystem
def_HttpEntry(ICON , req){
    fs::path p {"utils/favicon.ico"};
    if(fs::exists(p))
        return new FileResponse{p , "image/png"};
    else 
        return new HttpResponse{"404\nNOT FOUND" , HTTP_STATUS_404};        
}

// EXAMPLE 3.1 JsonResponse返回一个json格式的数据
//                 Apifox里有一个相关测试
def_HttpEntry(Check_It , req) {
    Json j;
    j.push_back({"Name" , "Bupt"}); //支持字符串类型
    j.push_back({"location" , { 
            {"E" , 116.20 } , 
            {"N" , 39.56} } 
        }); //支持结构体类型

    std::vector<std::string> strs {"Double" , "One" , "Stream"};
    j.push_back({"Grade" ,strs});               //支持来自vector的数组
    j.push_back({"Rate of Employment" , 99.9}); //支持数字（int, double)
    j.push_back({"Full of Budget" , false});    //支持 bool
    return new JsonResponse{j};
}

def_HttpEntry(SQL_Run , req){
    int errCode = 0;
    std::string val = "";
    int count = 0;
    std::string ans = req.getBody();
    if(ans == "__NULL__"){
        std::fstream fs("web/sqlService/database.html", std::ios::in | std::ios::binary);
        if(fs.is_open()) //std::cerr << "Return HTML\n";
        //std::cerr << "SQL RUN\n";
        return new FileResponse{fs , "text/html"};
    }
    
    std::cerr<<"["<<getGMTtime()<<"] "<<"SQL-req: "<<ans<<std::endl;
    const char *sql = ans.c_str();
    DB.Exec(sql);
    errCode = DB.ErrCode();
    val = DB.ReturnVal();
    count = DB.Count();
    return new HttpResponse{std::to_string(errCode) + "?" + std::to_string(count) + "&"+ val};
}

def_HttpEntry(SQL_Test,req){
    std::string sql = "select * from main;";
    int errcode = DB.Exec(sql.c_str());
    return new HttpResponse{std::to_string(errcode)};
}

static int id;

def_HttpEntry(SQL_Test2,req){
    id++;
    std::cerr<<"DataBase "<<id<<" In"<<std::endl;
    std::string sql = "drop table test2;create table test2(id int);";
    for(int i = 0;i<100;i++){
        sql += "insert into test2 values ("+std::to_string(i)+");";
    }
    for(int i = 30;i<60;i++){
        sql += "delete from test2 where id = "+std::to_string(i)+";";
    }
    sql += "select * from test2;";
    int errcode = DB.Exec(sql.c_str());
    std::cerr<<"DataBase "<<id<<" OK : "<<errcode<<std::endl;
    return new HttpResponse{std::to_string(errcode)};

}