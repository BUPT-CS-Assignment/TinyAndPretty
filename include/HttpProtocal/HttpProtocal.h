#ifndef __HTTP_MSG__
#define __HTTP_MSG__

#include <common.h>

////commonly used http status code
#define HTTP_STATUS_200 "HTTP/1.1 200 OK\r\n"
#define HTTP_STATUS_300 "HTTP/1.1 300 Multiple Choice\r\n"
#define HTTP_STATUS_400 "HTTP/1.1 400 Bad Request\r\n"
#define HTTP_STATUS_403 "HTTP/1.1 403 Forbidden\r\n"
#define HTTP_STATUS_404 "HTTP/1.1 404 Not Found\r\n"
#define HTTP_STATUS_502 "HTTP/1.1 502 Bad Gateway\r\n"

using Dict = std::vector<std::pair<std::string ,std::string>>;
////implementation of K-V string-pair dictionary
class StringDict {
	Dict item;
	size_t len;

	//init stringdict by two significant tokens
	void __init__(char * , const char * , const char *);
public : 
	StringDict() = default;
	//allow delayed construct
	StringDict(char *str , const char *token_1 , const char *token_2) {__init__(str , token_1 , token_2);}
	
	//query and fetch origin data in this dict. HttpException::NON_POS would occur when not found
	std::string& get(std::string_view);
	
	//query and fetch origin data in this dict. HttpException::NON_POS would occur when not found
	std::string& operator[] (const std::string& str) noexcept;
	
	//append one pair to dict in the end
	void push(std::string _fir , std::string _sec) ;
	
	//display this stringdict on stdout
	void show();    
	
	//calculate total length
	size_t length() const { return len;}

	//stringize to byte stream / string
	size_t stringize(char *buff);
};

////implementation of the item in multipart/form-fata
class FormData;
class FormItem {
friend class FormData;
	std::unique_ptr<StringDict> headers;
	std::unique_ptr<uint8_t[]>  data;
	std::string name     = "";
	std::string filename = "";
	size_t len;
public :
	//constructor by byte range
	FormItem(uint8_t*_begin , uint8_t* _end);

	//method of inverting tostring.			NOT guarantee correctness 
	operator std::string() const {return std::string((char *)data.get() , len);}
	
	//method of inverting to filestream.	NOT guarantee correctness 
	friend std:: fstream& operator << (std:: fstream& out , const FormItem& _this);
	friend std::ofstream& operator << (std::ofstream& out , const FormItem& _this);
	
	//calculate total length
	size_t length() 				 const {return len;}
	
	//query form key name
	std::string_view queryName() 	 const {return name;}
	
	//query form file name
	std::string_view queryFilename() const {return filename;}

	//display this form item on stdout in the way of char
	void show();
};

////implementation of the multipart/form-fata
class FormData {
	std::string boundary;
	std::vector<FormItem> form;
public : 
	FormData(std::string& , uint8_t* , size_t) ;
	
	//query form item by key nam
	FormItem& queryItem(std::string_view _name);
};

class Connection;
class HttpRequest {
	Connection *conn;

	std::unique_ptr<StringDict>  params;
	std::unique_ptr<StringDict>  headers;
	std::unique_ptr<FormData>    form;
	std::unique_ptr<uint8_t[]>   body;

	std::string method , path , version;

	size_t length ;
public : 
	HttpRequest(Connection * _conn , uint8_t* str, const size_t len);
	
	//query form data by item key name
	FormItem&   	 queryForm	( std::string_view _idx);

	//query parameters in url (GET only)
	std::string_view queryParam ( std::string_view _idx) noexcept; 
	
	//query header item
	std::string_view queryHeader( std::string_view _idx) noexcept; 
	
	//fetch and construct http body to string
	std::string 	 getBody() ;

	//query url path
	std::string_view Path() 	const noexcept { return path;}
	
	//query url method
	std::string_view Method() 	const noexcept { return method;}
	
	//query http protocal version
	std::string_view HttpVer() 	const noexcept { return version;}


	size_t Length() const { return length; }
};

class HttpResponseBase {
protected :
	std::string status                  = HTTP_STATUS_200;
	std::unique_ptr<StringDict> headers = std::make_unique<StringDict>();

	void setDefaultHeaders();
public :
	HttpResponseBase();
	HttpResponseBase( const std::string& _status);
	void appendHeader(const std::string _fir , const std::string _sec) ;

	auto stringize()
		->std::tuple<std::shared_ptr<uint8_t>, size_t>;
	virtual size_t length() const = 0;
	virtual size_t stringize(uint8_t **buff) = 0;
	virtual ~HttpResponseBase() {};
};


class HttpResponse : public HttpResponseBase{
	std::string body = "";
public :
	explicit HttpResponse() = default;
	explicit HttpResponse(std::string _body) ;
	explicit HttpResponse(std::string _body , const std::string _status);

	virtual size_t length() const override;
	virtual size_t stringize(uint8_t **buff) override;
};


class FileResponse : public HttpResponseBase{
    std::fstream body;
	size_t body_len = 0;
public :
    explicit FileResponse() = default;
    explicit FileResponse(std::fstream &_body , const std::string _type);
    explicit FileResponse(std::fstream &_body , const std::string _type , const std::string _status) ;
	explicit FileResponse(std::filesystem::path _file , const std::string _type);

	virtual size_t length() const override;
    virtual size_t stringize(uint8_t **buff) override;
};

#include <libs/SimpleJson.hpp>
using namespace SimpleJson;

class JsonResponse : public HttpResponseBase{
	Json body;
public :
	explicit JsonResponse() = default;
	explicit JsonResponse(Json &_body);
	explicit JsonResponse(Json &_body , const std::string _status);

	virtual size_t length() const override;
	virtual size_t stringize(uint8_t **buff) override;
};

#endif