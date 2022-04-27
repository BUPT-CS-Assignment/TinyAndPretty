#include <UserService/UserControl.h>
#include <UserService/Preload.h>

/* Sign In */
def_HttpEntry(SIGN_IN, req){
    std::string val = "", ans = req.getBody();
    if(ans == "__NULL__"){
        return new FileResponse{"web/UserService/SignIn.html" , "text/html"};
    }
    CONSOLE_LOG(0, "SignIn-Request '%s'\n", ans.c_str());
    /* Check SignIn Info */
    int index = ans.find_first_of("&");
    std::string id = ans.substr(0, index);
    std::string passwd = ans.substr(index + 1);
    int id_i;
    try{
        id_i = std::stoi(id);
    }
    catch(exception& e){
        return new HttpResponse{"19?0"};
    }
    User* user = UserStatus.AccurateLoacte(id_i);
    int res = -1;
    if(user == NULL){
        User newUser(id_i, USER_COMMON);
        res = newUser.SignIn(passwd);
        if(res == 0){
            CONSOLE_LOG(0, "New User '%d' Signed In\n",id_i);
            UserStatus.InsertData(id_i, newUser);
        }
    }
    else{
        if(user->getStatus() != USER_SIGN_IN){
            res = user->SignIn(passwd);
        }else{
            CONSOLE_LOG(0,"User '%d' Signed In Already\n",id_i);
            res = 0;
        }
    }
    return new HttpResponse{std::to_string(res) + "?1"};

}

/* Sign Up */
def_HttpEntry(SIGN_UP, req){
    std::string val = "", ans = req.getBody();
    if(ans == "__NULL__"){
        return new FileResponse{"web/UserService/SignUp.html" , "text/html"};
    }
    CONSOLE_LOG(0, "SignUp-Request '%s'\n", ans.c_str());
    /* Check Insert Info */
    int index = ans.find_first_of("&");
    std::string id = ans.substr(0, index);
    std::string passwd = ans.substr(index + 1);
    int id_i;
    try{
        id_i = std::stoi(id);
    }
    catch(exception& e){
        return new HttpResponse{"19?0"};
    }
    User newUser(id_i, USER_COMMON);
    int errcode = newUser.SignUp(passwd);
    //Check
    if(errcode != 0){
        return new HttpResponse{std::to_string(errcode) + "?0"};
    }
    return new HttpResponse{"0?1"};

}

User::User(){
    id_ = 0;
    auth_ = 0;
    status_ = 0;
}

User::User(int id, int auth){
    id_ = id;
    auth_ = auth;
    status_ = USER_SIGN_OUT;
}

int User::SignIn(std::string passwdInput){
    std::string id_s = std::to_string(id_);
    // Signing In //
    std::string sql = "select Passwd from UserToken where ID = " + std::to_string(id_) + ";";
    DB_ROOT.Exec(sql.c_str());
    int errCode = DB_ROOT.ErrCode();
    if(errCode != 0){
        return errCode;
    }
    std::string val = DB_ROOT.ReturnVal();
    val = val.substr(val.find(";") + 1);
    if(passwdInput != val){
        return 31;
    }
    status_ = USER_SIGN_IN;
    return CONSOLE_LOG(0, "User '%d' Signed In\n",id_);
}

int User::SignUp(std::string passwdInput){
    std::string id_s = std::to_string(id_);
    std::string sql = "insert into UserToken values (" + id_s + "," + passwdInput + ",0);";
    DB_ROOT.Exec(sql.c_str());
    return DB_ROOT.ErrCode();
}

int User::SignOut(){
    status_ = USER_SIGN_OUT;
    return 0;
}