#include <test/define.h>
#include <libs/NEdb.h>


def_HttpEntry(Link_Start){
    std::cerr << "FUNC START\n";
    std::string t = "";
    try{
        auto &ans = request.queryForm("BUG FILE");
    }
    catch(const HttpException &e){
        switch(e){
            case HttpException::NON_POS:
                t += "OMG , I FAIL!";
                break;
            case HttpException::NON_FORM:
                t += "Maybe I got sth. wrong...";
                break;
            default: break;
        }
    }
    // Json j;
    // j.push_back({"name" , "lzc"});
    // j.push_back({"age",18});
    // j.push_back({"email" ,"lingzichao@bupt.edu.cn"});

    std::cout << t.length() << std::endl;
    return new HttpResponse{t};
    //return new JsonResponse{j};
}


def_HttpEntry(Lent_Book){
    std::cerr << "Start Lent\n";

    std::fstream book("utils/out", std::ios::in | std::ios::binary);
    if(book.is_open()) std::cerr << "Find IT!\n";
    std::cerr << "Send Back\n";

    return new FileResponse{book , "application/pdf"};
}

def_HttpEntry(SQL_HELP){
    std::cerr<<"SQL Help Test\n";
    std::fstream fs("utils/sql_help.html", std::ios::in | std::ios::binary);
    if(fs.is_open()) std::cerr << "Return README\n";
    return new FileResponse{fs , "text/html"};


}

NEdb DB("/home/jianxf/.nesrc");

def_HttpEntry(SQL_Run){
    std::cerr << "SQL Run Test\n";
    std::string msg = "";
    std::string val = "";
    std::string ans = request.getBody();
    if(ans == "__NULL__"){
        std::fstream fs("utils/database.html", std::ios::in | std::ios::binary);
        if(fs.is_open()) std::cerr << "Return HTML\n";
        DB.dirInit();
        DB.scan();
        return new FileResponse{fs , "text/html"};
    }
    if(ans == "stop"){
        DB.close();
        return new HttpResponse{"DataBase Close&"};
    }
    const char *sql = ans.c_str();
    DB.exec(sql);
    msg = DB.getMsg();
    val = DB.getData();
    return new HttpResponse{msg + "&" + val};
}

