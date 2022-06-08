#ifndef __HTTP_MSG__
#define __HTTP_MSG__

#include "HttpBase.h"

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
	HttpRequest(Connection* _conn , uint8_t* str, const size_t len);
	
	/* query form data by item key name */
	FormItem&   	 queryForm	( std::string_view key );

	/*
		query parameters in url (GET only)
		is_insen : TRUE stands for 'Case insensitive'
	*/
	std::string_view queryParam ( std::string_view key , bool is_insen = false ) noexcept; 
	
	/* 
		query header item
		is_insen : TRUE stands for 'Case insensitive'
	*/
	std::string_view queryHeader( std::string_view key , bool is_insen = false ) noexcept; 
	
	/* fetch and construct http body to string */
	std::string 	 getBody() ;

	/* query url path */
	std::string_view Path() 	const noexcept { return path;}
	
	/* query url method */
	std::string_view Method() 	const noexcept { return method;}
	
	/* query http protocal version */
	std::string_view HttpVer() 	const noexcept { return version;}

	/* Connection */
	auto getConnection() { return conn; }
	std::string_view queryClientIP() const noexcept ;


	size_t Length() const { return length; }
};
#endif