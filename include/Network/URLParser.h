#ifndef __URL_PARSER__
#define __URL_PARSER__

#include <common.h>
#include <interfaces.h>

#include <functional>
#include <unordered_map>

using EntryFunc = std::function<HttpResponseBase* (HttpRequest&)>;

class URLParser {
private : 
    URLParser() ;
    URLParser(const URLParser&) ;
    URLParser(const URLParser&&) ;
    URLParser& operator = (const URLParser&&) ;
public :
    std::unordered_map<std::string_view , EntryFunc >url_table;
    
    static URLParser &getInstance() {
        static URLParser instance;
        return instance;
    }
};

#endif