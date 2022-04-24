#include <test/define.h>

/* Log In */
def_HttpEntry(LOG_IN, req){
    std::string msg = "";
    std::string val = "";
    int count = 0;
    std::string ans = req.getBody();
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
    DB.exec(sql.c_str());
    msg = DB.getMsg();
    val = DB.getData();
    val = val.substr(val.find(";") + 1);
    count = DB.getCount();
    //Check
    if(msg != "complete"){
        return new HttpResponse{msg + "?0"};
    }
    if(count == 1){
        if(passwd != val){
            count = 0;
        }
    }
    return new HttpResponse{msg + "?" + std::to_string(count)};
}

/* Sign Up */
def_HttpEntry(SIGN_UP, req){
    std::string msg = "";
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
    DB.exec(sql.c_str());
    msg = DB.getMsg();
    count = DB.getCount();
    //Check
    if(msg != "complete"){
        return new HttpResponse{msg + "?0"};
    }
    return new HttpResponse{msg + "?1"};

}