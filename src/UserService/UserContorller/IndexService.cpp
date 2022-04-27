#include <UserService/UserControl.h>
#include <UserService/Preload.h>

def_HttpEntry(UserIndex, req){
    std::string cookie(req.queryHeader("Cookie"));
    std::string function(req.queryHeader("Function"));
    std::string body = req.getBody();
    if(cookie == ""){
        return new HttpResponse{"-1?Access Denied"};
    }
    if(function == ""){
        return new FileResponse{"web/user/index.html","text/html"};
    }
    CONSOLE_LOG(0, "UserPanel-Req [cookie='%s', function='%s', body='%s']\n", cookie.c_str(),function.c_str(),body.c_str());
    /* Check SignIn Info */
    int idx = cookie.find("userid=");
    std::string id = cookie.substr(idx + 7, cookie.find(";",idx) - idx - 7);
    /* id check */
    int id_i;
    try{
        id_i = std::stoi(id);
    }
    catch(exception& e){
        return new HttpResponse{"-1?0"};
    }

    /* Cookie Check */
    User* user = COOKIE.AccurateLoacte(id_i);
    if(user == NULL) return new HttpResponse{"-1?0"};
    if(function == "Fetch"){
        std::string val = user->GetAllInfo();
        return new HttpResponse{"0?"+val};
    }else if(function == "Update" && body != "__NULL__"){
        int res = user->SetInfo(body);
        return new HttpResponse{std::to_string(res)+"?"};
    }else if(function == "SignOut"){
        user->SignOut();
        return new HttpResponse{"0?0"};
    }
    return new HttpResponse{"-1?0"};
    
}