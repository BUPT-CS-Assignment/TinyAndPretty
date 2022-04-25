#include <test/define.h>

/* Log In */
def_HttpEntry(LOG_IN, req){
    int errCode = 0;
    std::string val = "";
    int count = 0;
    std::string ans = req.getBody();
    std::cerr << req.Method();
    if(ans == "__NULL__"){
        std::fstream fs("web/userService/index.html", std::ios::in | std::ios::binary);
        if(fs.is_open()) //std::cerr << "Return HTML\n";
        //std::cerr << "SQL RUN\n";
            return new FileResponse{fs , "text/html"};
    }
    std::cerr << "[" << getGMTtime() << "] " << "LOGIN-req: " << ans << std::endl;
    int index = ans.find_first_of("&");
    std::string id = ans.substr(0, index);
    std::string passwd = ans.substr(index + 1);
    std::string sql = "select passwd from userInfo where id = " + id + ";";
    //Query
    DB.Exec(sql.c_str());
    errCode = DB.ErrCode();
    val = DB.ReturnVal();
    val = val.substr(val.find(";") + 1);
    count = DB.Count();
    //Check
    if(errCode != 0){
        return new HttpResponse{std::to_string(errCode) + "?0"};
    }
    if(count == 1){
        if(passwd != val){
            count = 0;
        }
    }
    return new HttpResponse{"0?" + std::to_string(count)};
}

/* Sign Up */
def_HttpEntry(SIGN_UP, req){
    int errCode = 0;
    int count = 0;
    std::string ans = req.getBody();
    if(ans == "__NULL__"){
        std::fstream fs("web/userService/signup.html", std::ios::in | std::ios::binary);
        if(fs.is_open()) //std::cerr << "Return HTML\n";
        //std::cerr << "SQL RUN\n";
            return new FileResponse{fs , "text/html"};
    }
    std::cerr << "[" << getGMTtime() << "] " << "LOGIN-req: " << ans << std::endl;
    int index = ans.find_first_of("&");
    std::string id = ans.substr(0, index);
    std::string passwd = ans.substr(index + 1);
    std::string sql = "insert into userInfo values (" + id + "," + passwd + ",0);";
    //Query
    DB.Exec(sql.c_str());
    errCode = DB.ErrCode();
    count = DB.Count();
    //Check
    if(errCode != 0){
        return new HttpResponse{std::to_string(errCode) + "?0"};
    }
    return new HttpResponse{"0?1"};

}