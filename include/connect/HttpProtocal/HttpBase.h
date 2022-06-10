#ifndef __HTTP_BASE__
#define __HTTP_BASE__

#include <common.h>

/* commonly used http status code */
#define HTTP_STATUS_200 "HTTP/1.1 200 OK\r\n"
#define HTTP_STATUS_202 "HTTP/1.1 202 Accepted\r\n"
#define HTTP_STATUS_204 "HTTP/1.1 204 No Content\r\n"
#define HTTP_STATUS_300 "HTTP/1.1 300 Multiple Choice\r\n"
#define HTTP_STATUS_400 "HTTP/1.1 400 Bad Request\r\n"
#define HTTP_STATUS_401 "HTTP/1.1 401 Unauthorized\r\n"
#define HTTP_STATUS_403 "HTTP/1.1 403 Forbidden\r\n"
#define HTTP_STATUS_404 "HTTP/1.1 404 Not Found\r\n"
#define HTTP_STATUS_502 "HTTP/1.1 502 Bad Gateway\r\n"

/* implementation of K-V string-pair dictionary */
class StringDict {

using Dict = std::vector<std::pair<std::string ,std::string>>;

	Dict item;
	size_t len = 0;

	/* init stringdict by two significant tokens */
	void __init__(char * , const char * , const char *) noexcept;
public : 
	StringDict() = default;
	/* allow delayed construct */
	StringDict(char* str , const char* token_1 , const char* token_2) {__init__(str , token_1 , token_2);}
	
	/* 
		query and fetch origin data in this dict. 
		is_insen : TRUE stands for 'Case insensitive'
	   	HttpException::NON_POS would occur when not found  
	*/
	std::string& get(std::string_view _key, bool is_insen = false);
	
	/* query and fetch origin data in this dict. HttpException::NON_POS would occur when not found  */
	std::string& operator[] (std::string_view str) noexcept;
	
	/* append one pair to dict in the end */
	void push(std::string _fir , std::string _sec) noexcept;
	
	/* display this stringdict on stdout */
	void show();    
	
	/* calculate total length */
	size_t length() const { return len; }

	/* stringize to byte stream / string */
	size_t stringize(char *buff);
};

/* implementation of the item in multipart/form-fata */
class FormData;
class FormItem {
friend class FormData;
	std::unique_ptr<StringDict> headers;
	std::unique_ptr<uint8_t[]>  data;
	std::string name     = "";
	std::string filename = "";
	size_t len;
public :
	/* constructor by byte range */
	FormItem(uint8_t*_begin , uint8_t* _end);

	/* method of inverting tostring. NOT guarantee correctness  */
	operator std::string() const {return std::string((char *)data.get() , len);}
	
	/* method of inverting to filestream.	NOT guarantee correctness  */
	friend std:: fstream& operator << (std:: fstream& out , const FormItem& _this);
	friend std::ofstream& operator << (std::ofstream& out , const FormItem& _this);
	
	/* calculate total length */
	size_t length() 				 const {return len;}
	
	/* query form key name */
	std::string_view queryName() 	 const {return name;}
	
	/* query form file name */
	std::string_view queryFilename() const {return filename;}

	/* display this form item on stdout in the way of char */
	void show();
};

/* implementation of the multipart/form-fata */
class FormData {
	std::string boundary;
	std::vector<FormItem> form;
public : 
	FormData(std::string& , uint8_t* , size_t) ;
	
	/* query form item by key nam */
	FormItem& queryItem(std::string_view _name);
};

#endif