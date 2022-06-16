#include <test/define.h>
// EXAMPLE 0 
// 如果需要添加一个入口， 需要在三处文件修改 （以后会简化吧...）：
// 1. interfaces.h
// 2. router.conf
// 3. test.cpp (your own .cpp)

// EXAMPLE 1.0 HttpRequest 的一些基本使用,默认以text/html，参数为std::string
def_HttpEntry(Link_Start , request){
    std::string t;
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
            t += "<h2>NOTHING!</h2>";
            t += "<h3>no laughing matter... </h3>";
            break;
        default: break;
        }
    }
   return new HttpResponse{t};
}


// EXAMPLE 2.1 FileResponse支持直接从指定文件路径发送文件，效率比较高，但是仅为只读文件
def_HttpEntry(Lent_Book , req){
    return new FileResponse{"utils/out" , "application/pdf"};
}


// EXAMPLE 2.3 FileResponse建议使用c++17的std::filesystem
// def_HttpEntry(Icon , req){
//     fs::path p {"utils/favicon.ico"};
//     if(fs::exists(p))
//         return new FileResponse{p , "image/png"};
//     else 
//         return new HttpResponse{"404\nNOT FOUND" , HTTP_STATUS_404};        
// }

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

def_HttpEntry(MD5Test,req){
    std::string val = "";
    std::string ans = req.getBody();
    
    UTILSTD::CONSOLE_LOG(true,"* api/md5 called [res:%s]\n",ans.c_str());

    std::string str_md5 = md5(ans);

    UTILSTD::CONSOLE_LOG(true,"Process Result : %s\n",str_md5.c_str());

    return new HttpResponse{str_md5};
}

def_HttpEntry(LogCheck,req){
    UTILSTD::CONSOLE_LOG(true,"* api/log called\n");
    return new FileResponse{".server/log","text/html"};
}

def_HttpEntry(CometTest , req) {
    //Timer::createComet(req);
    std::string ans {};
    
    auto table = Timer::getVirtualTime();
    for(auto i : table) {
        ans += std::to_string(i) += " ";
    }

    return new HttpResponse{ans};
}

def_HttpEntry(CometBroad , req) {
    Timer::launchBroadCast(new HttpResponse {"Hello , everyone\n"});

    return new HttpResponse {"Successful launch"};
}