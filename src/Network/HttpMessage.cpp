#include <Network/HttpProtocal.h>
#include <Network/URLParser.h>
#include <Network/ServerBase.h>
#include <Network/HttpException.h>

char *nsplit(char *str, const char *token, int n);
std::string getGMTtime();

#define NOW_POS ((char *)str + cur)
#define CUR_MOV(obj, offset) (cur += (obj.length() + offset))

/*---------------------------------HttpRequest---------------------------------*/


HttpRequest::HttpRequest(Connection *_conn, uint8_t *str, const size_t len) : conn(_conn)
{
#ifdef DEBUG
    std::cerr << "---------------In HttpRequest---------------" << std::endl;
#endif
    size_t cur = 0;
    method = nsplit((char *)str, " ", 1);
    CUR_MOV(method, 1);
    path = nsplit(NOW_POS, " ", 1);
    nsplit(NOW_POS, "?", 1);
    if (size_t t_len = path.length(); strlen(NOW_POS) != t_len)
    {
        path = NOW_POS;
        params = std::make_unique<StringDict>(NOW_POS, "=", "&");
        cur += t_len + 1;
    }
    else CUR_MOV(path, 1);

    if (URLParser::getInstance().preCheck(path, method))
        throw HttpException::NON_PATH;
    version = nsplit(NOW_POS, "\r\n", 2);
    CUR_MOV(version, 2);

    headers = std::make_unique<StringDict>(nsplit(NOW_POS, "\r\n\r\n", 4), ": ", "\r\n");
    CUR_MOV((*headers), 4);

    length = len - cur;
    try
    {
        std::string_view content_length = queryHeader("Content-Length");
#ifdef DEBUG
        std::cerr << "In Header Length: "
                  << content_length << "\nReal Length: "
                  << length << std::endl;
#endif
        if (content_length != "" && content_length != std::to_string(length))
            throw HttpException::ERROR_LEN;
    }
    catch (const HttpException &e) { throw e; }
   
    if (length > 0)
    {
        body = std::make_unique<uint8_t[]>(length);
        memcpy(body.get(), NOW_POS, length);
        try
        {
            std::string &type = headers->get("Content-type");
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
#ifdef DEBUG
    std::cerr << "Connection Status : " << status << "\n";
#endif
    if (status != "keep-alive")
        conn->setCloseFlag();
#ifdef DEBUG
    std::cerr << "---------------HttpRequest Finish---------------" << std::endl;
#endif
}

FormItem& HttpRequest::queryForm ( std::string_view _idx) 
{
    if(form == nullptr) throw HttpException::NON_FORM;
    return form->queryItem(_idx);
}


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
/*---------------------------------HttpResponseBase---------------------------------*/


void HttpResponseBase::setDefaultHeaders()
{
    if (headers == nullptr)
        return;
    headers->push("Date", getGMTtime());
    headers->push("Server", "Cjango/1.0");
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
#ifdef DEBUG
        std::cerr << "Get FIle : " <<cur << "\n";
#endif
        if(cur == buff_size) 
            *buff = (uint8_t *)realloc(*buff , (buff_size <<= 1) ) ;
    }
    body.close();

    //low posibility bug;
    strcpy((char *)*buff + cur , "\r\n\0\r\n");
    cur += 5;
    body_len = cur - body_len;
#ifdef DEBUG
    std::cerr << "File Info : \nBuff_size : " << buff_size << " File Size : " << body_len << "\n";
#endif
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
    strcpy((char *)*buff + cur , "\r\n\0\r\n");
    cur += 5;
    return cur;
}