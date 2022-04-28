#include <UserService/UserControl.h>
#include <UserService/Preload.h>
using namespace NEDBSTD;
using namespace UTILSTD;
using namespace std;

/* Sign In */
def_HttpEntry(SignIn, req){
    /* Request Dispose */
    std::string cookie(req.queryHeader("Cookie"));
    std::string body = req.getBody();
    if(body == "__NULL__" || cookie == ""){
        return new FileResponse{"web/user/signin.html" , "text/html"};
    }
    CONSOLE_LOG(0,1,1,"Signin-Req [cookie='%s', body='%s']\n", cookie.c_str(), body.c_str());

    /* Check SignIn Info */
    int idx = cookie.find("userid=");
    std::string id = cookie.substr(idx + 7, cookie.find_first_of(";") - idx - 8);
    std::string passwd = body.substr(body.find("passwd=") + 7);

    /* id check */
    int id_i;
    try{
        id_i = std::stoi(id);
    }
    catch(exception& e){
        return new HttpResponse{"19?0"};
    }

    /* Cookie Check */
    User* user = COOKIE.AccurateLoacte(id_i);
    int res;
    if(user == NULL){
        User newUser(id_i, USER_COMMON);
        res = newUser.SignIn(passwd);
        if(res == 0){
            CONSOLE_LOG(0,1,1,"New User '%d' Signed In\n", id_i);
            COOKIE.InsertData(id_i, newUser);
        }
    }
    else{
        if(user->getStatus() != USER_SignIn){
            res = user->SignIn(passwd);
        }
        else{
            CONSOLE_LOG(0,1,1,"User '%d' Signed In Already\n", id_i);
            res = 0;
        }
    }
    HttpResponse* resp = new HttpResponse(std::to_string(res) + "?1");
    resp->appendHeader("Cookie","userid="+id+"; path=/user/");
    return resp;

}

/* Sign Up */
def_HttpEntry(SignUp, req){
    std::string val = "", ans = req.getBody();
    if(ans == "__NULL__"){
        return new FileResponse{"web/user/signup.html" , "text/html"};
    }
    CONSOLE_LOG(0,1,1,"SignUp-Request '%s'\n", ans.c_str());
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
