#include <test/define.h>

def_HttpEntry(Link_Start){
    //std::cerr << "FUNC START\n";
    std::string t = "";
    try {
        std::string ans = request.queryForm("Key");
        t += ans;
        ans = request.queryForm("Action");
        t += ans;
    } catch (const HttpException& e) {
        switch (e)
        {
        case HttpException::NON_POS :
            t += "OMG , I FAIL!";
            break;
        case HttpException::NON_FORM:
            t += "Maybe I got sth. wrong...";
            break;
        default: break;
        }
    }
    //Json j;
    // j.push_back({"name" , "lzc"});
    // j.push_back({"age",18});
    // j.push_back({"email" ,"lingzichao@bupt.edu.cn"});
    std::cout << t << std::endl;

    //std::cout << t.length() << std::endl;
    return new HttpResponse{t};
    //return new JsonResponse{j};
}


def_HttpEntry(Lent_Book){
    //std::cerr << "Start Lent\n";
    std::fstream book("utils/out", std::ios::in | std::ios::binary);
    //if(book.is_open()) std::cerr << "Find IT!\n";
    //std::cerr << "Sending Back\n";

    return new FileResponse{book , "application/pdf"};
}

def_HttpEntry(SQL_HELP){
    std::fstream fs("utils/sql_help.html", std::ios::in | std::ios::binary);
    if(fs.is_open()){
        //std::cerr << "SQl HELP\n";
        return new FileResponse{fs , "text/html"};
    }else{
        return new HttpResponse{"404\nNOT FOUND"};
    }


}

def_HttpEntry(ICON){
    std::fstream fs("utils/favicon.ico", std::ios::in | std::ios::binary);
    if(fs.is_open()){
        return new FileResponse{fs , "image/png"};
    }else{
        return new HttpResponse{"NOT FOUND"};
    }
}

def_HttpEntry(SQL_Run){
    std::string msg = "";
    std::string val = "";
    std::string ans = request.getBody();
    if(ans == "__NULL__"){
        std::fstream fs("utils/database.html", std::ios::in | std::ios::binary);
        if(fs.is_open()) //std::cerr << "Return HTML\n";
        //std::cerr << "SQL RUN\n";
        return new FileResponse{fs , "text/html"};
    }
    std::cerr<<"SQL-req: "<<ans<<std::endl;
    const char *sql = ans.c_str();
    DB.exec(sql);
    msg = DB.getMsg();
    val = DB.getData();
    return new HttpResponse{msg + "&" + val};
}

