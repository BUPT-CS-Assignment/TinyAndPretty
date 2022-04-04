#include <test/define.h>


def_HttpEntry(Link_Start) {
    std::cerr << "FUNC START\n";
    std::string t = "<h1>New Stage</h1>";
    try {
        auto& ans = request.queryForm("Lzy");
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
    Json j;
    j.push_back({"name" , "lzc"});
    j.push_back({"age",18});
    j.push_back({"email" ,"lingzichao@bupt.edu.cn"});

    std::cout << t << std::endl;
    //return new HttpResponse{t};
    return new JsonResponse{j};
}
