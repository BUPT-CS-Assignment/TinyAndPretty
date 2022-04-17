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

def_HttpEntry(DataBase_Test){
    std::cerr << "Start DataBase Test\n";
    std::string t = "";
    NEdb DB("/home/jianxf/.nesrc");
    if(!DB.dirInit()){
        t = DB.getMsg();
    }
    else{
        if(!DB.exec("create table tap_test(id int,name text,email text);\
                    insert into tap_test values (0,user1,email_test1@tap.com);\
                    insert into tap_test values (1,user2,email_test2@tap.com);\
                    select * from tap_test;\
        ")){
            t = DB.getMsg();
        }else{
            t = DB.getData();
        }
        std::cerr<<DB.getMsg()<<std::endl;
        DB.exec("drop table tap_test;");
    }
    DB.close();
    return new HttpResponse{t};

}