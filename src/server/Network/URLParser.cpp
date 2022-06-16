#include <connect/Network/URLParser.h>
#include <interfaces.h>

#define ADD_URL(_url, _func) url_table.emplace(_url, _func);

extern std::string getGMTtime(uint32_t offset = 0);
extern EntryFunc StaticResponse;

URLParser::URLParser()
{
#include <router.conf>

}

EntryFunc& URLParser::URLparse(std::string_view _url) 
{
    try {
        return url_table.at( _url );
        
    }   catch(const std::out_of_range& e) {
        UTILSTD::CONSOLE_LOG(true,"unknow url '%s'\n", _url.data());
        return StaticResponse;
    }
}

bool URLParser::preCheck(std::string_view _url, std::string_view _method) noexcept
{
    if (_method == "GET" && _url == static_url)
        return true;
    else
        return false;

    return (url_table.find(_url) == url_table.end());
}