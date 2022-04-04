#ifndef __HTTP_MSG__
#define __HTTP_MSG__

#include <common.h>

#define HTTP_STATUS_200 "HTTP/1.1 200 OK\r\n"
#define HTTP_STATUS_300 "HTTP/1.1 300 Multiple Choice\r\n"
#define HTTP_STATUS_400 "HTTP/1.1 400 Bad Request\r\n"
#define HTTP_STATUS_403 "HTTP/1.1 403 Forbidden\r\n"
#define HTTP_STATUS_404 "HTTP/1.1 404 Not Found\r\n"
#define HTTP_STATUS_502 "HTTP/1.1 502 Bad Gateway\r\n"

enum class HttpException {NON_POS , ERROR_LEN , OUT_OF_LIMIT, NON_PATH , NON_CONN , NON_FORM};

using Dict = std::vector<std::pair<std::string ,std::string>>;

class StringDict {
	Dict item;
	size_t len;

	void __init__(char * , const char * , const char *);
public : 
	StringDict() = default;
	StringDict(char *str , const char *token_1 , const char *token_2) {__init__(str , token_1 , token_2);}
	
	std::string& get(std::string_view);
	std::string& operator[] (const std::string& str) ;
	void push(std::string _fir , std::string _sec) ;
	size_t length() const { return len;}
	void show();    

	size_t stringize(char *buff);
	//virtual ~StringDict() {};
};
class FormData;
class FormItem {
friend class FormData;
	std::unique_ptr<StringDict> headers;
	std::unique_ptr<uint8_t[]>  data;
	std::string name     = "";
	std::string filename = "";
	size_t len;
public :
	FormItem(uint8_t*_begin , uint8_t* _end);
	operator std::string() const {return std::string((char *)data.get());}
	friend std:: fstream& operator << (std:: fstream& out , const FormItem& _this);
	friend std::ofstream& operator << (std::ofstream& out , const FormItem& _this);
	size_t length() const {return len;}
	std::string_view queryName() const {return name;}
	void show();
};

class FormData {
	std::string boundary;
	std::vector<FormItem> form;
public : 
	FormData(std::string& , uint8_t* , size_t) ;
	
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
	
	FormItem&   	 queryForm	( std::string_view _idx);
	std::string_view queryParam ( std::string_view _idx) noexcept; 
	std::string_view queryHeader( std::string_view _idx) noexcept; 

	std::string_view Path()     const { return path;}
	std::string_view Method()   const { return method;}
	std::string_view HttpVer()  const { return version;}

	size_t Length() const { return length; }
};

class HttpResponseBase {
protected :
	std::string status                  = HTTP_STATUS_200;
	std::unique_ptr<StringDict> headers = std::make_unique<StringDict>();

	void setDefaultHeaders();
public :
	HttpResponseBase();
	HttpResponseBase(std::string& _status);
	void appendHeader(std::string _fir , std::string _sec) ;

	virtual size_t length() const = 0;
	virtual size_t stringize(uint8_t **buff) = 0;
	virtual ~HttpResponseBase() {};
};


class HttpResponse : public HttpResponseBase{
	std::string body = "";
public :
	explicit HttpResponse() = default;
	explicit HttpResponse(std::string _body) ;
	explicit HttpResponse(std::string _body , std::string _status);

	virtual size_t length() const override;
	virtual size_t stringize(uint8_t **buff) override;
};


// class FileResponse : public HttpResponseBase{
//     Json body;
// public :
//     explicit FileResponse() = default;
//     explicit FileResponse(Json &_body) : body( std::move(_body) ) {};
//     explicit FileResponse(Json &_body , const std::string _status) : 
//                             HttpResponseBase(_status) , body( std::move( _body )) {};

//     virtual size_t stringize(char *buff) override;
// };

#include "SimpleJson.hpp"
using namespace SimpleJson;

class JsonResponse : public HttpResponseBase{
	Json body;
public :
	explicit JsonResponse() = default;
	explicit JsonResponse(Json &_body);
	explicit JsonResponse(Json &_body , std::string _status);

	virtual size_t length() const override;
	virtual size_t stringize(uint8_t **buff) override;
};

#endif