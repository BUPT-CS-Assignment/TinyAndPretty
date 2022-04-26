#include <UserService/UserControl.h>
#include <UserService/Preload.h>

/* Sign In */
def_HttpEntry(SIGN_IN, req){
    int errCode = 0, count = 0;
    std::string val = "", ans = req.getBody();
    if(ans == "__NULL__"){
        return new FileResponse{"web/UserService/SignIn.html" , "text/html"};
    }
    CONSOLE_LOG(0, "SignIn-Request '%s'\n", ans.c_str());
    /* Check SignIn Info */
    int index = ans.find_first_of("&");
    std::string id = ans.substr(0, index);
    std::string passwd = ans.substr(index + 1);
    std::string sql = "select Passwd from UserToken where ID = " + id + ";";
    //Query
    DB_ROOT.Exec(sql.c_str());
    errCode = DB_ROOT.ErrCode();
    val = DB_ROOT.ReturnVal();
    val = val.substr(val.find(";") + 1);
    count = DB_ROOT.Count();
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
    int errCode = 0, count = 0;
    std::string val = "", ans = req.getBody();
    if(ans == "__NULL__"){
        return new FileResponse{"web/UserService/SignUp.html" , "text/html"};
    }
    CONSOLE_LOG(0, "SignUp-Request '%s'\n", ans.c_str());
    /* Check Insert Info */
    int index = ans.find_first_of("&");
    std::string id = ans.substr(0, index);
    std::string passwd = ans.substr(index + 1);
    std::string sql = "insert into UserToken values (" + id + "," + passwd + ",0);";
    //Query
    DB_ROOT.Exec(sql.c_str());
    errCode = DB_ROOT.ErrCode();
    count = DB_ROOT.Count();
    //Check
    if(errCode != 0){
        return new HttpResponse{std::to_string(errCode) + "?0"};
    }
    return new HttpResponse{"0?1"};

}