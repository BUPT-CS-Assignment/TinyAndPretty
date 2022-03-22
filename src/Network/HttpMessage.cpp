#include <Network/HttpProtocal.h>

char *nsplit(char *str ,const char *token , int n);

HttpRequest::HttpRequest(const char *str) {
    headers["Content-Type"] = "text/html";
    
    char msg [ strlen(str) + 1 ];
    ERROR(msg != nullptr , "Error Malloc");
    strcpy(msg , str);

    method  = nsplit(msg , " " , 1);
    path    = nsplit(nullptr , " " , 1);
    version = nsplit(nullptr , "\r\n" , 2);
    headers.__init__( nsplit(nullptr , "\r\n\r\n" , 4) , ": " , "\r\n"); 
    body    = nsplit(nullptr , "\0" , 1);

    strcpy(msg , path.c_str());
    nsplit(msg , "?" , 1);
    if(strlen(msg) != path.length()) {
        path = msg;
        params.__init__(nsplit(nullptr , "" , 0) , "=" ,"&");
        params.show();
    }

}

size_t HttpResponse::stringize(char *buff) {
    size_t cur = 0;

    strcpy(buff , status.c_str());      cur += status.length();
    cur += headers.stringize(buff + cur);
    strcpy(buff + cur , body.c_str());  cur += body.length();

    return cur;
}

size_t JsonResponse::stringize(char *buff) {
    size_t cur = 0;
    
    strcpy(buff , status.c_str());      cur += status.length();
    cur += headers.stringize(buff + cur);
    cur +=    body.stringize(buff + cur);

    return cur;
}