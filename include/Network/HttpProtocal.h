#ifndef __HTTP_MSG__
#define __HTTP_MSG__

#include <common.h>

using Dict = std::vector<std::pair<std::string ,std::string>>;

class StringDict {
protected : 
    Dict item;
    size_t len;
public : 
    StringDict() = default;
    virtual void __init__(char * , const char * , const char *);
    std::string& get(const std::string&);
    std::string& operator[] (const std::string& str) ;
    size_t length() const {return len;}
    void show();    
};

class HttpHeader : public StringDict{
    Dict cookies;
public :
    HttpHeader() = default;
    size_t stringize(char *buff) ;
};


#define HTTP_STATUS_200 "HTTP/1.1 200 OK\r\n"
#define HTTP_STATUS_300 "HTTP/1.1 300 Multiple Choice\r\n"
#define HTTP_STATUS_400 "HTTP/1.1 400 Bad Request\r\n"
#define HTTP_STATUS_403 "HTTP/1.1 403 Forbidden\r\n"
#define HTTP_STATUS_404 "HTTP/1.1 404 Not Found\r\n"
#define HTTP_STATUS_502 "HTTP/1.1 502 Bad Gateway\r\n"

class HttpResponseBase {
public :
    std::string status = HTTP_STATUS_200;
    HttpHeader headers;

    HttpResponseBase() = default;
    HttpResponseBase(const std::string &_status) : status(_status) {};

    virtual size_t stringize(char *buff) = 0;
};

class HttpRequest {
public : 
    StringDict  params;
    HttpHeader headers;
    std::string method , body , path , version;

    HttpRequest(const char *str);
    std::string& Path() { return path;}
};


class HttpResponse : public HttpResponseBase{
public :
    std::string body = "";
    explicit HttpResponse() = default;
    explicit HttpResponse(const std::string _body) : body(_body) {};
    explicit HttpResponse(const std::string _body , const std::string _status) : 
                            HttpResponseBase(_status) , body(_body) {};

    virtual size_t stringize(char *buff) override;
};

#include "SimpleJson.hpp"
using namespace SimpleJson;
class JsonResponse : public HttpResponseBase{
public :
    Json body;
    explicit JsonResponse() = default;
    explicit JsonResponse(Json &_body) : body( std::move(_body) ) {};
    explicit JsonResponse(Json &_body , const std::string _status) : 
                            HttpResponseBase(_status) , body( std::move( _body )) {};

    virtual size_t stringize(char *buff) override;
};

#endif