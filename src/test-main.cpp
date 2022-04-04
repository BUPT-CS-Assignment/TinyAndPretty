/*
    This file will be linked to your test module 
    when MAKE mode turn to 'debug',and then generate
    an executable program on '/build/$(MODULE_NAME)/'
    Please use 'make MODE=debug {run}?'
*/
#include <common.h>
#include <Network/Network.h>
char fault[] = 
"GET /PATH?key=1&rest=pass HTTP/1.1\r\n"
"Date: Wed\r\n"
"Connection: Keep-Alive\r\n"
"Content-Type: text/html\r\n\r\n"
;

char fault2[] = {
"/?key=1&test=pass"
};


using std::cout;
// class JsonObject {
// public:
//     virtual void Func(){ cout << "Base\n";}
// };

// class JsonItem : public JsonObject
// {
// public:
//     std::string t;
//     JsonItem(const char * _t) :t(_t){}
//     void Func(){cout << "Derived\n";}
// };


int main() {
    // 不安全的转换
    std::string a = "aaaaaa";
    std::string_view b= a;
    cout << b << "\n";
    a = "bbbbbb";
    cout << b;
    return 0;
}