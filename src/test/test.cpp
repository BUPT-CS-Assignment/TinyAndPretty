#include <test/define.h>
// EXAMPLE 0 
// 如果需要添加一个入口， 需要在三处文件修改 （以后会简化吧...）：
// 1. interfaces.h
// 2. router.conf
// 3. test.cpp (your own .cpp)

void stringConcat(std::string& tar){;}  //recursion ending
template<typename T , typename... Args>
void stringConcat(std::string& tar , T value , Args... args){
	tar += std::string{value};
	stringConcat(tar , args...);
}

// EXAMPLE 1.0 HttpRequest 的一些基本使用,默认以text/html，参数为std::string
def_HttpEntry(Link_Start , request){
    std::string t {};

    stringConcat( t ,
    "<body><h1>Hello ! Dear Friend ,</h1>" ,
    "<h3>I know you comes from ...",request.queryClientIP(),"!</h3> " // 查询客户IP
    "<h3>And also you comes by ...",request.HttpVer()," <-With-> ",request.Method(),"!</h3> ", // 查询http版本和请求方法
    "<h3>What's more, you have ...",request.queryHeader("Content-Length"),"bytes !</h3> ");   // 根据key查询http header；

    stringConcat(t , 
    "<h1>Wow ! It seems that you Bring me a PRESENT</h1>",
    "<h3> There are",request.Length(),"gifts for me ,thank you! : ) </h3> " ,
    "<h3>Let me see... </h3>");
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
            t += "<h1>NOTHING!</h1>";
            t += "<h3>no laughing matter... </h3>";
            break;
        default: break;
        }
    }

    stringConcat(t , 
    "<h1>Look up! There are patameters following you!</h1>",
    "<h3> It is : id = " ,request.queryParam("id") , " </h3> " ,// 查询url参数。若没有为""(空串)
    "<a href=\"https://github.com/NoUITeam/TinyAndPretty\">Click ME for more info! </a></body>");
    return new HttpResponse{t};
}


// EXAMPLE 2.1 FileResponse支持直接从指定文件路径发送文件，效率比较高，但是仅为只读文件
def_HttpEntry(Lent_Book , req){
    return new FileResponse{"utils/out" , "application/pdf"};
}


// EXAMPLE 2.3 FileResponse建议使用c++17的std::filesystem
def_HttpEntry(ICON , req){
    fs::path p {"utils/favicon.ico"};
    if(fs::exists(p))
        return new FileResponse{p , "image/png"};
    else 
        return new HttpResponse{"404\nNOT FOUND" , HTTP_STATUS_404};        
}

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

def_HttpEntry(MD5_Test,req){
    std::string val = "";
    std::string ans = req.getBody();
    if(ans == "__NULL__"){
        return new FileResponse{"utils/md5test.html" , "text/html"};
    }
    
    std::cerr<<"["<<getGMTtime()<<"] "<<"MD5-req: "<<ans<<std::endl;
    std::string str_md5 = md5(ans);
    std::cerr<<"MD5 Process Result : "<<str_md5<<std::endl;
    return new HttpResponse{str_md5};
}

def_HttpEntry(WECHAT_TOKEN,req){
    return new FileResponse{"utils/wechatToken.txt","text"};
}


