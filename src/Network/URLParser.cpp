#include <Network/URLParser.h>
#include <Network/HttpException.h>
#include <interfaces.h>

#define SET_STATIC(_url) 
#define ADD_URL(_url, _func) url_table.emplace( _url ,  _func );

EntryFunc StaticResponse = [](HttpRequest& AS){
    return new HttpResponse("FILE");
};

URLParser::URLParser() {
#include <router.conf>

}

EntryFunc& URLParser::URLparse( std::string_view _url) {
    std::cerr<<"URL-req:"<<_url<<std::endl;
    if(_url == static_url) return StaticResponse;
    else try {
        return url_table.at( _url );
    } catch(const std::out_of_range& e) {
        throw HttpException::NON_PATH;
    }
}


bool URLParser::preCheck( std::string_view _url , std::string_view _method ) noexcept{ 
    if(_method == "GET" &&  _url == static_url ) return true;
    else return false;
    
    return (url_table.find(_url) == url_table.end());
} 