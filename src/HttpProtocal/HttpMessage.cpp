#include <HttpProtocal/HttpProtocal.h>
#include <Network/URLParser.h>
#include <Network/ServerBase.h>
#include <HttpProtocal/HttpException.h>

char *nsplit(char *str, const char *token, int n);
static bool RequestLengthChecker(HttpRequest* re , size_t real) noexcept;
std::string getGMTtime();

#define NOW_POS ((char *)str + cur)
#define CUR_MOV(obj, offset) (cur += (obj.length() + offset))

/*---------------------------------HttpRequest---------------------------------*/

//
HttpRequest::HttpRequest(Connection *_conn, uint8_t *str, const size_t len) : conn(_conn)
{
    IFDEBUG(std::cerr << "---------------In HttpRequest---------------" << std::endl);
    size_t cur = 0;

    //split url method
    method = nsplit((char *)str, " ", 1);
    CUR_MOV(method, 1);

    //split url path
    path = nsplit(NOW_POS, " ", 1);

    //split parameters in url 
    nsplit(NOW_POS, "?", 1);
    if (size_t t_len = path.length(); strlen(NOW_POS) != t_len)
    {
        path = NOW_POS;
        params = std::make_unique<StringDict>(NOW_POS, "=", "&");
        cur += t_len + 1;
    }
    else CUR_MOV(path, 1);

    //previously check whether url path exists
    if (URLParser::getInstance().preCheck(path, method))
        throw HttpException::NON_PATH;

    //split http version
    version = nsplit(NOW_POS, "\r\n", 2);
    CUR_MOV(version, 2);

    //split http header
    headers = std::make_unique<StringDict>(nsplit(NOW_POS, "\r\n\r\n", 4), ": ", "\r\n");
    CUR_MOV((*headers), 4);

    length = len - cur; // calculate body length
    if( !RequestLengthChecker(this , length) )
        throw HttpException::ERROR_LEN;
    

    if (length > 0) // if has body
    {
        body = std::make_unique<uint8_t[]>(length);
        memcpy(body.get(), NOW_POS, length);
        try
        {   //split body to multipart/form-data
            std::string &type = headers->get("Content-Type");

            if (type.find("multipart/form-data") != type.npos)
            {   
                size_t t_pos = type.find("boundary");
                std::string boundary = type.substr(t_pos + 9); // 9 : sizeof "boundary="
                type = "multipart/form-data";
                form = std::make_unique<FormData>(boundary, str + cur, length);
            }
        }
        catch (const HttpException &e) { ; }
    }
    std::string_view status = queryHeader("Connection");
    IFDEBUG(std::cerr << "Connection Status : " << status << "\n");

    if (status != "keep-alive")
        conn->setCloseFlag();
    IFDEBUG(
        std::cerr << "---------------HttpRequest Finish---------------" << std::endl;
    );
}

// check whether length in header is equal to the real
static bool RequestLengthChecker(HttpRequest* re , size_t real) noexcept{

    std::string_view content_length = re->queryHeader("Content-Length");
    IFDEBUG(
        std::cerr << "In Header Length: "
                << content_length << "\nReal Length: "
                << real << std::endl;
    );
    if (content_length != "" && content_length != std::to_string(real))
        return false;
    return true;
}

FormItem& HttpRequest::queryForm ( std::string_view _idx)
{
    if(form == nullptr) 
        throw HttpException::NON_FORM;
    return form->queryItem(_idx);
}

//query parameters in url (GET only)
std::string_view HttpRequest::queryParam(std::string_view _idx) noexcept
{
    if (params == nullptr)
        return std::string_view("");

    try
    {
        return params->get(_idx);
    }
    catch (const HttpException &e)
    {
        return std::string_view("");
    }
}
std::string_view HttpRequest::queryHeader(std::string_view _idx) noexcept
{
    if (headers == nullptr)
        return std::string_view("");

    try
    {
        return headers->get(_idx);
    }
    catch (const HttpException &e)
    {
        return std::string_view("");
    }
}

std::string HttpRequest::getBody()
{
    if(body != nullptr) return std::string((char*)body.get(),length);
    return "__NULL__";
}

/*---------------------------------HttpResponseBase---------------------------------*/


void HttpResponseBase::setDefaultHeaders()
{
    if (headers == nullptr)
        return;
    headers->push("Date", getGMTtime());
    headers->push("Server", "TINYandPRETTY/1.1");
    headers->push("Connection", "keep-alive");
}

HttpResponseBase::HttpResponseBase(const std::string &_status) : status(std::move(_status))
{
    setDefaultHeaders();
}

HttpResponseBase::HttpResponseBase()
{
    setDefaultHeaders();
}

void HttpResponseBase::appendHeader(std::string _fir, std::string _sec)
{
    headers->push(std::move(_fir), std::move(_sec));
}

auto HttpResponseBase::stringize()
    -> std::tuple<std::shared_ptr<uint8_t>, size_t> 
{
   uint8_t* data = nullptr;
   size_t len = stringize(&data);
   return std::make_tuple( std::shared_ptr<uint8_t>(data) , len);
}

/*---------------------------------HttpResponse---------------------------------*/


HttpResponse::HttpResponse(std::string _body) : HttpResponseBase(), body(std::move(_body))
{
    appendHeader("Content-Type", "text/html; charset=utf-8");
    appendHeader("Content-Length" , std::to_string( body.length() ));
}

HttpResponse::HttpResponse(std::string _body, std::string _status) : HttpResponseBase(_status), body(_body)
{
    appendHeader("Content-Type", "text/html; charset=utf-8");
    appendHeader("Content-Length" , std::to_string( body.length() ));
}

size_t HttpResponse::length() const
{
    return (status.length() + headers->length() + body.length());
}

size_t HttpResponse::stringize(uint8_t **buff)
{
    size_t buff_size = BUFF_INIT_SIZE;
    size_t cur = 0;

    *buff = (uint8_t *)calloc(1 , buff_size) ;

    strcpy((char *)(*buff), status.c_str());
    cur += status.length();
    cur += headers->stringize((char *)*buff + cur);
    
    if(buff_size < cur + body.length() )
        *buff =  (uint8_t *)realloc(*buff , (buff_size = cur + body.length() + 1) ) ;

    strcpy((char *)(*buff) + cur, body.c_str());
    cur += body.length();
    return cur;
}

/*---------------------------------FileResponse---------------------------------*/

FileResponse::FileResponse(std::fstream &_body , const std::string _type) : HttpResponseBase() , body( std::move(_body) ) 
{
    appendHeader("Content-Type" , _type);
}
FileResponse::FileResponse(std::fstream &_body , const std::string _type , const std::string _status) 
                                                    : HttpResponseBase(_status), body(std::move(_body))
{
    appendHeader("Content-Type" , _type);
}

FileResponse::FileResponse(std::filesystem::path _file , const std::string _type) 
{

}


size_t FileResponse::length() const 
{
    return (status.length() + headers->length() + body_len);    
}

size_t FileResponse::stringize(uint8_t **buff) 
{
    size_t buff_size = BUFF_INIT_SIZE;
    size_t cur = 0;

    *buff = (uint8_t *)calloc(1 , buff_size) ;

    strcpy((char *)(*buff), status.c_str());
    cur += status.length();
    cur += headers->stringize((char *)*buff + cur);


    body_len = cur;
    body.seekg(0 , std::ios::beg);
    while(!body.eof()) {
        body.read( (char *)*buff + cur , buff_size - cur);
        cur += body.gcount();
        if(cur == buff_size) 
            *buff = (uint8_t *)realloc(*buff , (buff_size <<= 1) ) ;
    }
    body.close();

    //low posibility bug;
    strcpy((char *)*buff + cur , "\r\n\0\r\n");
    cur += 5;
    body_len = cur - body_len;
    IFDEBUG(
        std::cerr << "File Info : \nBuff_size : " << buff_size 
                  << " File Size : " << body_len << "\n"
    );
    return cur;
}

/*---------------------------------JsonResponse---------------------------------*/

JsonResponse::JsonResponse(Json &_body) : HttpResponseBase() , body(std::move(_body))
{
    appendHeader("Content-Type", "application/json");
}

JsonResponse::JsonResponse(Json &_body, std::string _status) : HttpResponseBase(_status), body(std::move(_body))
{
    appendHeader("Content-Type", "application/json");
}

size_t JsonResponse::length() const
{
    return (status.length() + headers->length() + body.length());
}

size_t JsonResponse::stringize(uint8_t **buff)
{
    size_t buff_size = BUFF_INIT_SIZE;
    size_t cur = 0;

    *buff = (uint8_t *)calloc(1 , buff_size) ;

    strcpy((char *)*buff, status.c_str());
    cur += status.length();
    cur += headers->stringize((char *)*buff + cur);

    const char *tmp = body.stringize();
    if(buff_size < cur + body.length() )
        *buff =  (uint8_t *)realloc(*buff , (buff_size = cur + body.length() + 1) ) ;
    memcpy(*buff + cur , tmp , body.length());
    cur += body.length();

    //low posibility bug;
    // strcpy((char *)*buff + cur , "\r\n\0\r\n");
    // cur += 5;
    return cur;
}