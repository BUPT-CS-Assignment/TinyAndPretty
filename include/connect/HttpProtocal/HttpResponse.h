#ifndef __HTTP_RESP__
#define __HTTP_RESP__

#include "HttpBase.h"

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
	virtual ~HttpResponseBase() {}
};


class HttpResponse : public HttpResponseBase{
	std::string body = "";
public :
	explicit HttpResponse() = default;
	explicit HttpResponse(std::string _body) ;
	explicit HttpResponse(std::string _body , const std::string _status);
    explicit HttpResponse(std::string _body, const std::string _msg, const std::string _status);

	virtual size_t length() const override;
	virtual size_t stringize(uint8_t **buff) override;
};

class FileResponse : public HttpResponseBase{
	//verison 1 : r/w fstream
    std::fstream body;
	//version 2 : read only file 
	fs::path filepath;
	
	size_t body_len = 0;
public :
    explicit FileResponse() = default;
    explicit FileResponse(std::fstream &_body , const std::string _type);
    explicit FileResponse(std::fstream &_body , const std::string _type , const std::string _status) ;

	virtual size_t length() const override;
    virtual size_t stringize(uint8_t **buff) override;

	/* ---- newly appended ---- */
	/* more efficient way to tansfer static read-only file */
	explicit FileResponse(fs::path _filepath  , const std::string _type);
	explicit FileResponse(fs::path _filepath, const std::string _type, const std::string _msg);

	fs::path  getFilepath() const {return filepath;}
	auto stringizeHeader()
		-> std::tuple<std::shared_ptr<uint8_t> , size_t>;
};

#include <libs/SimpleJson.hpp>
using namespace SimpleJson;

class JsonResponse : public HttpResponseBase{
	Json body;
public :
	explicit JsonResponse() = default;
	explicit JsonResponse(Json &_body);
    explicit JsonResponse(Json &_body, const std::string _msg, const std::string _status);
	explicit JsonResponse(Json &_body , const std::string _status);

	virtual size_t length() const override;
	virtual size_t stringize(uint8_t **buff) override;
};


#endif