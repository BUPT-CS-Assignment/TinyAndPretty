#ifndef __URL_PARSER__
#define __URL_PARSER__

#include <common.h>
#include <functional>
#include <unordered_map>

#include <connect/HttpProtocal/HttpProtocal.h>
using EntryFunc = std::function<HttpResponseBase* (HttpRequest&)>;


// non thread safe!
class URLParser {
private : 
    URLParser() ;
    URLParser(const URLParser&) ;
    URLParser(const URLParser&&) ;
    URLParser& operator = (const URLParser&&) ;
    std::string_view static_url = "";
    std::unordered_map<std::string_view , EntryFunc >url_table;
public :
    
    static URLParser &getInstance() {
        static URLParser instance;
        return instance; 
    }
    EntryFunc& URLparse( std::string_view _url) ;
    bool preCheck( std::string_view _url  , std::string_view _method = "") noexcept;
};

#endif