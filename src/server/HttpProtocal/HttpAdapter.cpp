#include <connect/HttpProtocal/HttpAdapter.h>
#include <connect/HttpProtocal/HttpResponse.h>
#include <connect/Network/ServerBase.h>
#include <exception>

extern size_t preFetchLength(const char * str);
extern size_t preFetchHeader(const char * str);

/* receive raw byte stream */
auto HttpAdapter::recvHttpData(Connection *conn)
	-> std::tuple< std::shared_ptr<uint8_t> , size_t > 
{
	size_t 	 len  = 0;
	uint8_t* data = nullptr;
	len = sock->recvPeekData(conn->getFD() , &data);

	// handle GET method
	if ( !strncmp((char *)data , "GET" , 3) ) {
		len = sock->recvNonBlockData(conn->getFD() , &data);

	// handle POST method
	} else if ( !strncmp((char *)data , "POST" , 4) ) {
		len =  preFetchLength( (char *)data );
		len += preFetchHeader( (char *)data );
		len =  sock->recvCertainData(conn->getFD() , &data , len + 4); // 4 : size of "\r\n\r\n"

	// generally handle
	} else
		len = sock->recvData(conn->getFD() , &data);

	return std::make_tuple( 
		std::shared_ptr<uint8_t>{data} , 
		len);
}

/* file type specilized check ; true for filesystem response */
bool HttpAdapter::branchFilePathResp(
	Connection *conn , 
	std::shared_ptr<HttpResponseBase> ret)
{
	// File response checker
	auto file_resp = std::dynamic_pointer_cast<FileResponse> (ret);
	if( file_resp == nullptr ) 	return false;

	// version checker
	fs::path target = file_resp->getFilepath();
	if(target == "" ) 			return false;
	
	// hanle filesystem type FileResponse
	auto [buff, slen] = file_resp->stringizeHeader();

	return sock->sendFileWithHeader(
		conn->getFD() , 
		target.c_str() , 
		buff.get() , 
		slen);
}

void HttpAdapter::sendHttpData(
	Connection *conn , 
	std::shared_ptr<HttpResponseBase> ret) 
{
	if (ret == nullptr) return ;
	// Successfully get response(even exception occurs)

	// specialized 
	if (branchFilePathResp(conn , ret)) return ;
	// Send back to socket 
	auto [buff, slen] = ret->stringize();

	slen = sock->sendData(conn->getFD(), buff.get(), slen);
}
